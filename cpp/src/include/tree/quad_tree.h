#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "common/types/rect.h"
#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTree {
 public:
  QuadTree() = delete;
  QuadTree(const QuadTree &other) = delete;

  explicit QuadTree(const InsideHandler &inside_handler);

  auto IsEmpty() const -> bool;

  void Build(Rect rect, const std::vector<Geometry2DType> &objects);

 private:
  auto Build(int depth, Rect rect, const std::vector<Geometry2DType> &objects)
      -> std::optional<std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>>>;

  std::optional<std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>>> root_{std::nullopt};
  Rect rect_;
  InsideHandler inside_handler_;
  std::vector<Geometry2DType> split_objects_[MAX_CHILDREN];
};

}  // namespace utilities
