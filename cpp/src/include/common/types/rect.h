#pragma once

#include <iostream>

#include "common/types/vector2d.h"

namespace utilities {
class Rect {
 public:
  Rect() = default;
  ~Rect() = default;

  explicit Rect(Vector2D left_down, Vector2D right_up);
  Rect(const Rect &other) = default;
  auto operator=(const Rect &other) -> Rect &;

  auto GetLeftDown() const -> Vector2D;
  auto GetRightUp() const -> Vector2D;
  auto GetWidth() const -> double;
  auto GetHeight() const -> double;

  friend auto operator<<(std::ostream &os, const Rect &rect) -> std::ostream &;

 private:
  Vector2D left_down_, right_up_;
};

}  // namespace utilities
