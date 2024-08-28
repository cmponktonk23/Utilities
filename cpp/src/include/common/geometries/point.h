#pragma once

#include <iostream>

#include "common/geometries/shape2d.h"
#include "common/geometries/vector2d.h"

namespace utilities {

class Point : public Shape2D {
 public:
  Point() = default;
  ~Point() = default;

  explicit Point(Vector2D pos);
  explicit Point(double x, double y);
  Point(const Point &other) = default;
  auto operator=(const Point &other) -> Point &;

  auto GetPos() const -> Vector2D;

  auto Interact(const Shape2D &other) const -> bool override;

  friend auto operator<<(std::ostream &os, const Point &rect) -> std::ostream &;

 private:
  Vector2D pos_;
};

}  // namespace utilities
