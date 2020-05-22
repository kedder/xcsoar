/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef GAUGE_VARIO_H
#define GAUGE_VARIO_H

#include "Screen/AntiFlickerWindow.hpp"
#include "Screen/BulkPoint.hpp"
#include "Blackboard/FullBlackboard.hpp"
#include "Math/Point2D.hpp"

struct VarioLook;
class ContainerWindow;

class GaugeVario : public AntiFlickerWindow
{
  static constexpr unsigned NARROWS = 3;
  static constexpr int YOFFSET = 36;

  /** 5 m/s */
  static constexpr int GAUGEVARIORANGE = 5;

  /** degrees total sweep */
  static constexpr int GAUGEVARIOSWEEP = 90;

  static constexpr int gmax = GAUGEVARIOSWEEP + 2;
  static constexpr int gmin = -gmax;

  struct BallastGeometry {
    PixelRect label_rect, value_rect;
    PixelPoint label_pos, value_pos;

    BallastGeometry() = default;
    BallastGeometry(const VarioLook &look, const PixelRect &rc) noexcept;
  };

  struct BugsGeometry {
    PixelRect label_rect, value_rect;
    PixelPoint label_pos, value_pos;

    BugsGeometry() = default;
    BugsGeometry(const VarioLook &look, const PixelRect &rc) noexcept;
  };

  struct Geometry {
    unsigned nlength0, nlength1, nwidth, nline;

    IntPoint2D offset;

    PixelPoint top_position;
    PixelPoint middle_position;
    PixelPoint bottom_position;

    BallastGeometry ballast;
    BugsGeometry bugs;

    Geometry() = default;
    Geometry(const VarioLook &look, const PixelRect &rc) noexcept;
  } geometry;

  struct DrawInfo {
    bool initialised = false;
    PixelRect rc;
    PixelPoint text_position;
    double last_value;
    TCHAR last_text[32];
    Unit last_unit;
  };

  struct LabelValueDrawInfo {
    DrawInfo label;
    DrawInfo value;

    void Reset() noexcept {
      label.initialised = false;
      value.initialised = false;
    }
  };

  const FullBlackboard &blackboard;

  const VarioLook &look;

  bool dirty = true;

  bool background_dirty = true;
  bool needle_initialised = false;

  LabelValueDrawInfo average_di, mc_di, gross_di;

  int ival_av_last = 0;
  int vval_last = 0;
  int sval_last = 0;
  int ival_last = 0;

  double last_v_diff = 0;

  int last_ballast = -1;

  int last_bugs = -1;

  BulkPixelPoint polys[(gmax * 2 + 1) * 3];
  BulkPixelPoint lines[gmax * 2 + 1];

public:
  GaugeVario(const FullBlackboard &blackboard,
             ContainerWindow &parent, const VarioLook &look,
             PixelRect rc, const WindowStyle style=WindowStyle()) noexcept;

protected:
  const MoreData &Basic() const noexcept {
    return blackboard.Basic();
  }

  const DerivedInfo &Calculated() const noexcept {
    return blackboard.Calculated();
  }

  const ComputerSettings &GetComputerSettings() const noexcept {
    return blackboard.GetComputerSettings();
  }

  const GlidePolar &GetGlidePolar() const noexcept {
    return GetComputerSettings().polar.glide_polar_task;
  }

  const VarioSettings &Settings() const noexcept {
    return blackboard.GetUISettings().vario;
  }

protected:
  /* virtual methods from class Window */
  virtual void OnResize(PixelSize new_size) override;

  /* virtual methods from class AntiFlickerWindow */
  virtual void OnPaintBuffer(Canvas &canvas) override;

private:
  void RenderZero(Canvas &canvas) noexcept;
  void RenderValue(Canvas &canvas, PixelPoint position,
                   LabelValueDrawInfo &di,
                   double Value, const TCHAR *Label) noexcept;
  void RenderSpeedToFly(Canvas &canvas, int x, int y) noexcept;
  void RenderBallast(Canvas &canvas) noexcept;
  void RenderBugs(Canvas &canvas) noexcept;
  int  ValueToNeedlePos(double Value) noexcept;
  void RenderNeedle(Canvas &canvas, int i, bool average, bool clear) noexcept;
  void RenderVarioLine(Canvas &canvas, int i, int sink, bool clear) noexcept;
  void RenderClimb(Canvas &canvas) noexcept;

  void MakePolygon(const int i) noexcept;
  void MakeAllPolygons() noexcept;
  BulkPixelPoint *getPolygon(const int i) noexcept;
};

#endif
