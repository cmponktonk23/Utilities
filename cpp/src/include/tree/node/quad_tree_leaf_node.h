#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeLeafNode : public QuadTreeNode<Geometry2DType, InsideHandler> {
  friend class QuadTree<Geometry2DType, InsideHandler>;

 public:
  QuadTreeLeafNode() = delete;
  QuadTreeLeafNode(const QuadTreeLeafNode &other) = delete;

  ~QuadTreeLeafNode() override = default;

  explicit QuadTreeLeafNode(Rect rect, const InsideHandler &inside_handler, size_t depth);

  void Print() override;

 private:
  std::vector<Geometry2DType> objects_;
};

}  // namespace utilities
