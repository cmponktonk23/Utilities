#pragma once

namespace utilities {

enum class Shape2DType {
  INVALID_SHAPE = 0,
  POINT,
  RECT,
};

class Shape2D {
 public:
  Shape2D() = default;
  virtual ~Shape2D() = default;

  virtual auto Interact(const Shape2D &) const -> bool = 0;

  auto GetShapeType() const -> Shape2DType { return shape_type_; }

  size_t id_;

 protected:
  explicit Shape2D(Shape2DType type) : shape_type_(type) {}

 private:
  Shape2DType shape_type_;
};

}  // namespace utilities
