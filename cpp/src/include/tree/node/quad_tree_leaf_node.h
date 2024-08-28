#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "tree/node/quad_tree_node.h"

namespace utilities {

class QuadTreeLeafNode : public QuadTreeNode {
  friend class QuadTree;

 public:
  QuadTreeLeafNode() = delete;
  QuadTreeLeafNode(const QuadTreeLeafNode &other) = delete;

  ~QuadTreeLeafNode() override = default;

  explicit QuadTreeLeafNode(Rect rect, size_t depth);

  void Print() override;

 private:
  std::vector<Shape2D> objects_;
};

}  // namespace utilities
