#pragma once

#include <iostream>
#include <vector>
#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeLeafNode : public QuadTreeNode<Geometry2DType, InsideHandler> {
 public:
  QuadTreeLeafNode() = delete;
  QuadTreeLeafNode(const QuadTreeLeafNode &other) = delete;

  ~QuadTreeLeafNode() override;

  explicit QuadTreeLeafNode(Rect rect, const InsideHandler &inside_handler);

  void Insert(const Geometry2DType &obj);
  void Insert(const std::vector<Geometry2DType> &objects);

 private:
  std::vector<Geometry2DType> objects_;
};
}  // namespace utilities
