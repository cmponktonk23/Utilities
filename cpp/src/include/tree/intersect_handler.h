#include "common/geometries/point.h"
#include "common/geometries/rect.h"

namespace utilities {

class RectRectInteractHandler {
 public:
  auto operator()(const Rect &rect1, const Rect &rect2) const -> bool;
};

class PointRectInteractHandler {
 public:
  auto operator()(const Point &point, const Rect &rect) const -> bool;
};

class PointPointInteractHandler {
 public:
  auto operator()(const Point &point1, const Point &point2) const -> bool;
};

}  // namespace utilities
