#include "common/types/rect.h"

namespace utilities {

Rect::Rect(Vector2D left_down, Vector2D right_up) : left_down_(left_down), right_up_(right_up) {}

auto Rect::operator=(const Rect &other) -> Rect & {
  if (this == &other) {
    return *this;
  }

  left_down_ = other.left_down_;
  right_up_ = other.right_up_;

  return *this;
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
