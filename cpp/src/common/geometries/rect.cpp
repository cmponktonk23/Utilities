#include "common/geometries/rect.h"
#include "common/exception.h"
#include "tree/intersect_handler.h"

namespace utilities {

Rect::Rect(Vector2D left_down, Vector2D right_up)
    : Shape2D(Shape2DType::RECT), left_down_(left_down), right_up_(right_up) {}

auto Rect::operator=(const Rect &other) -> Rect & {
  if (this == &other) {
    return *this;
  }

  left_down_ = other.left_down_;
  right_up_ = other.right_up_;

  return *this;
}

auto Rect::Interact(const Shape2D &other) const -> bool {
  if (other.GetShapeType() == Shape2DType::RECT) {
    return RectRectInteractHandler()(dynamic_cast<const Rect &>(*this), dynamic_cast<const Rect &>(other));
  } else if (other.GetShapeType() == Shape2DType::POINT) {
    return PointRectInteractHandler()(dynamic_cast<const Point &>(other), dynamic_cast<const Rect &>(*this));
  } else {
    throw NotImplementedException("interact with other shapes is not supported");
  }
}

auto Rect::GetLeftDown() const -> Vector2D { return left_down_; }

auto Rect::GetRightUp() const -> Vector2D { return right_up_; }

auto Rect::GetWidth() const -> double { return right_up_.GetX() - left_down_.GetX(); }

auto Rect::GetHeight() const -> double { return right_up_.GetY() - left_down_.GetY(); }

auto operator<<(std::ostream &os, const Rect &rect) -> std::ostream & {
  os << "Rect[(" << rect.left_down_.GetX() << ", " << rect.left_down_.GetY() << "), (" << rect.right_up_.GetX() << ", "
     << rect.right_up_.GetY() << ")]";
  return os;
}

}  // namespace utilities
