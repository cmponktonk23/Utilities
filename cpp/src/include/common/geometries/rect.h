#pragma once

#include <iostream>

#include "common/geometries/shape2d.h"
#include "common/geometries/vector2d.h"

namespace utilities {

class Rect : public Shape2D {
 public:
  explicit Rect();
  ~Rect() = default;

  explicit Rect(Vector2D left_down, Vector2D right_up);
  Rect(const Rect &other) = default;
  auto operator=(const Rect &other) -> Rect &;

  auto GetLeftDown() const -> Vector2D;
  auto GetRightUp() const -> Vector2D;
  auto GetWidth() const -> double;
  auto GetHeight() const -> double;

  auto Interact(const Shape2D &other) const -> bool override;

  friend auto operator<<(std::ostream &os, const Rect &rect) -> std::ostream &;

 private:
  Vector2D left_down_, right_up_;
};

}  // namespace utilities
