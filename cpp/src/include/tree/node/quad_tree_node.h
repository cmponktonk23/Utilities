#pragma once

#include <iostream>

#include "common/geometries/rect.h"
#include "common/geometries/vector2d.h"

namespace utilities {

enum class QuadTreeNodeType { INVALID_NODE = 0, LEAF_NODE, INTERNAL_NODE };

// forward declaration to avoid cyclic dependency
class QuadTree;

class QuadTreeNode {
  friend class QuadTree;

 public:
  // Delete all constructor / destructor to ensure memory safety
  QuadTreeNode() = delete;
  QuadTreeNode(const QuadTreeNode &other) = delete;

  virtual ~QuadTreeNode() = default;

  explicit QuadTreeNode(Rect rect, size_t depth);

  auto IsLeafNode() const -> bool;
  void SetNodeType(QuadTreeNodeType node_type);

  auto IsContain(const Shape2D &obj) const -> bool;

  virtual void Print(){};

 protected:
  QuadTreeNodeType node_type_;
  Rect rect_;
  size_t depth_;
};

}  // namespace utilities
