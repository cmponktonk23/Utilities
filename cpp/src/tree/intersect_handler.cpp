#include "tree/intersect_handler.h"

namespace utilities {

auto RectRectInteractHandler::operator()(const Rect &rect1, const Rect &rect2) const -> bool {
  auto x_min_1 = rect1.GetLeftDown().GetX();
  auto x_max_1 = rect1.GetRightUp().GetX();
  auto y_min_1 = rect1.GetLeftDown().GetY();
  auto y_max_1 = rect1.GetRightUp().GetY();

  auto x_min_2 = rect2.GetLeftDown().GetX();
  auto x_max_2 = rect2.GetRightUp().GetX();
  auto y_min_2 = rect2.GetLeftDown().GetY();
  auto y_max_2 = rect2.GetRightUp().GetY();

  bool x_overlap = (x_min_1 <= x_max_2 && x_max_1 >= x_min_2);
  bool y_overlap = (y_min_1 <= y_max_2 && y_max_1 >= y_min_2);

  return x_overlap && y_overlap;
}

auto PointRectInteractHandler::operator()(const Point &point, const Rect &rect) const -> bool {
  auto px = point.GetPos().GetX();
  auto py = point.GetPos().GetY();

  auto rx1 = rect.GetLeftDown().GetX();
  auto rx2 = rect.GetRightUp().GetX();
  auto ry1 = rect.GetLeftDown().GetY();
  auto ry2 = rect.GetRightUp().GetY();

//   std::cout << (rx1 <= px && px <= rx2 && ry1 <= py && py <= ry2) << std::endl;

  return rx1 <= px && px <= rx2 && ry1 <= py && py <= ry2;
}

auto PointPointInteractHandler::operator()(const Point &point1, const Point &point2) const -> bool { return true; }

}  // namespace utilities