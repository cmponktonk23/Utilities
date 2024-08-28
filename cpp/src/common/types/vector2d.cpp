#include "common/types/vector2d.h"

namespace utilities {

Vector2D::Vector2D() : x_(0), y_(0) {}

Vector2D::Vector2D(double x, double y) : x_(x), y_(y) {}

auto Vector2D::operator=(const Vector2D &other) -> Vector2D & {
  if (this == &other) {
    return *this;
  }

  x_ = other.x_;
  y_ = other.y_;

  return *this;
}

auto Vector2D::operator+(const Vector2D &other) const -> Vector2D { return Vector2D(x_ + other.x_, y_ + other.y_); }

auto Vector2D::operator-(const Vector2D &other) const -> Vector2D { return Vector2D(x_ - other.x_, y_ - other.y_); }

auto Vector2D::operator*(double scalar) const -> Vector2D { return Vector2D(x_ * scalar, y_ * scalar); }

auto Vector2D::operator+=(const Vector2D &other) -> Vector2D & {
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

auto Vector2D::operator-=(const Vector2D &other) -> Vector2D & {
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

auto Vector2D::operator*=(double scalar) -> Vector2D & {
  x_ *= scalar;
  y_ *= scalar;
  return *this;
}

auto Vector2D::operator==(const Vector2D &other) const -> bool { return x_ == other.x_ && y_ == other.y_; }

auto Vector2D::operator!=(const Vector2D &other) const -> bool { return !(*this == other); }

auto Vector2D::GetX() const -> double { return x_; }

auto Vector2D::GetY() const -> double { return y_; }

}  // namespace utilities
