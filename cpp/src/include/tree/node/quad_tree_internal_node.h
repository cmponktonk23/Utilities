#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeInternalNode : public QuadTreeNode<Geometry2DType, InsideHandler> {
  friend class QuadTree<Geometry2DType, InsideHandler>;

 public:
  QuadTreeInternalNode() = delete;
  QuadTreeInternalNode(const QuadTreeInternalNode &other) = delete;

  ~QuadTreeInternalNode() override = default;

  explicit QuadTreeInternalNode(Rect rect, const InsideHandler &inside_handler, size_t depth);

  void Print() override;

 private:
  std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>> children_[4];
};

}  // namespace utilities
