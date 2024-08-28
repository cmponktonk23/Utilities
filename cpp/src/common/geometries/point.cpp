#include "common/geometries/point.h"
#include "common/exception.h"
#include "common/geometries/rect.h"
#include "tree/intersect_handler.h"

namespace utilities {

Point::Point(Vector2D pos) : Shape2D(Shape2DType::POINT), pos_(pos) {}
Point::Point(double x, double y) : Point(Vector2D(x, y)) {}

auto Point::operator=(const Point &other) -> Point & {
  if (this == &other) {
    return *this;
  }

  pos_ = other.pos_;

  return *this;
}

auto Point::GetPos() const -> Vector2D { return pos_; }

auto Point::Interact(const Shape2D &other) const -> bool {
  if (other.GetShapeType() == Shape2DType::RECT) {
    return PointRectInteractHandler()(dynamic_cast<const Point &>(*this), dynamic_cast<const Rect &>(other));
  } else if (other.GetShapeType() == Shape2DType::POINT) {
    return PointPointInteractHandler()(dynamic_cast<const Point &>(other), dynamic_cast<const Point &>(*this));
  } else {
    throw NotImplementedException("interact with other shapes is not supported");
  }
}

auto Point::GetPos() const -> Vector2D { return pos_; }

auto operator<<(std::ostream &os, const Point &point) -> std::ostream & {
  os << "Rect[(" << point.pos_.GetX() << ", " << point.pos_.GetY() << ")]";
  return os;
}

}  // namespace utilities
