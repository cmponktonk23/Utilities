#include "tree/inside_handler.h"

namespace utilities {

auto RectInsideRectHandler::operator()(const Rect &rect1, const Rect &rect2) const -> bool {
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

}  // namespace utilities