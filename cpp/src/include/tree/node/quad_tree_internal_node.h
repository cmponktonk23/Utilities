#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeInternalNode : public QuadTreeNode<Geometry2DType, InsideHandler> {
 public:
  QuadTreeInternalNode() = delete;
  QuadTreeInternalNode(const QuadTreeInternalNode &other) = delete;

  ~QuadTreeInternalNode() override;

  explicit QuadTreeInternalNode(Rect rect, const InsideHandler &inside_handler);

  std::optional<std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>>> children_[MAX_CHILDREN];
};

}  // namespace utilities
