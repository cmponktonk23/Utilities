#pragma once

namespace utilities {
class Vector2D {
 public:
  Vector2D();
  ~Vector2D() = default;

  explicit Vector2D(double x, double y);
  Vector2D(const Vector2D &other);
  Vector2D &operator=(const Vector2D &other);

  Vector2D operator+(const Vector2D &other) const;
  Vector2D operator-(const Vector2D &other) const;
  Vector2D operator*(double scalar) const;

  Vector2D &operator+=(const Vector2D &other);
  Vector2D &operator-=(const Vector2D &other);
  Vector2D &operator*=(double scalar);

  bool operator==(const Vector2D &other) const;
  bool operator!=(const Vector2D &other) const;

  auto GetX() const -> double;
  auto GetY() const -> double;

 private:
  double x_;
  double y_;
};

}  // namespace utilities
