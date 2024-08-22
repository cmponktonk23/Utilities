#pragma once

#include "common/types/vector2d.h"

namespace utilities {
class Rect {
 public:
  Rect() = default;
  ~Rect() = default;

  explicit Rect(Vector2D left_down, Vector2D right_up);
  Rect(const Rect &other);
  Rect &operator=(const Rect &other);

  auto GetLeftDown() const -> Vector2D;
  auto GetRightUp() const -> Vector2D;
  auto GetWidth() const -> double;
  auto GetHeight() const -> double;

 private:
  Vector2D left_down_, right_up_;
};

}  // namespace utilities
