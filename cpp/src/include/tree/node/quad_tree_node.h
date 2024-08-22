#pragma once

#include <iostream>
#include "common/types/rect.h"
#include "common/types/vector2d.h"

namespace utilities {

#define MAX_CHILDREN 4
#define MAX_DEPTH 10

enum class QuadTreeNodeType { INVALID_NODE = 0, LEAF_NODE, INTERNAL_NODE };

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeNode {
 public:
  // Delete all constructor / destructor to ensure memory safety
  QuadTreeNode() = delete;
  QuadTreeNode(const QuadTreeNode &other) = delete;

  virtual ~QuadTreeNode() { std::cout << "clear QuadTreeNode" << std::endl; }

  explicit QuadTreeNode(Rect rect, const InsideHandler &inside_handler);

  auto IsLeafNode() const -> bool;
  void SetNodeType(QuadTreeNodeType node_type);

  auto IsContain(Geometry2DType obj) const -> bool;

 private:
  QuadTreeNodeType node_type_;
  Rect rect_;
  InsideHandler inside_handler_;
};

}  // namespace utilities
