#pragma once

#include <iostream>

#include "common/types/rect.h"
#include "common/types/vector2d.h"

namespace utilities {

enum class QuadTreeNodeType { INVALID_NODE = 0, LEAF_NODE, INTERNAL_NODE };

// forward declaration to avoid cyclic dependency
template <typename Geometry2DType, typename InsideHandler>
class QuadTree;

template <typename Geometry2DType, typename InsideHandler>
class QuadTreeNode {
  friend class QuadTree<Geometry2DType, InsideHandler>;

 public:
  // Delete all constructor / destructor to ensure memory safety
  QuadTreeNode() = delete;
  QuadTreeNode(const QuadTreeNode &other) = delete;

  virtual ~QuadTreeNode() = default;

  explicit QuadTreeNode(Rect rect, const InsideHandler &inside_handler, size_t depth);

  auto IsLeafNode() const -> bool;
  void SetNodeType(QuadTreeNodeType node_type);

  auto IsContain(const Geometry2DType &obj) const -> bool;

  virtual void Print(){};

 protected:
  QuadTreeNodeType node_type_;
  Rect rect_;
  InsideHandler inside_handler_;
  size_t depth_;
};

}  // namespace utilities
