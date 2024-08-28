#include "common/types/rect.h"

namespace utilities {
class RectInsideRectHandler {
 public:
  auto operator()(const Rect &rect1, const Rect &rect2) const -> bool;
};

}  // namespace utilities
