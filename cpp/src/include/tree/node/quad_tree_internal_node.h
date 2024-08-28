#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "tree/node/quad_tree_node.h"

namespace utilities {

class QuadTreeInternalNode : public QuadTreeNode {
  friend class QuadTree;

 public:
  QuadTreeInternalNode() = delete;
  QuadTreeInternalNode(const QuadTreeInternalNode &other) = delete;

  ~QuadTreeInternalNode() override = default;

  explicit QuadTreeInternalNode(Rect rect, size_t depth);

  void Print() override;

 private:
  std::unique_ptr<QuadTreeNode> children_[4];
};

}  // namespace utilities
