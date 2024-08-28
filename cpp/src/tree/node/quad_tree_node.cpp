#include "tree/node/quad_tree_node.h"
#include "tree/inside_handler.h"

namespace utilities {

QuadTreeNode::QuadTreeNode(Rect rect, size_t depth)
    : rect_(rect), inside_handler_(inside_handler), depth_(depth) {}

auto QuadTreeNode::IsLeafNode() const -> bool {
  return this->node_type_ == QuadTreeNodeType::LEAF_NODE;
}

void QuadTreeNode::SetNodeType(QuadTreeNodeType node_type) {
  this->node_type_ = node_type;
}

auto QuadTreeNode::IsContain(const Geometry2DType &obj) const -> bool {
  return this->inside_handler_(obj, rect_);
}

}  // namespace utilities
