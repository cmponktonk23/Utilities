#include "tree/node/quad_tree_node.h"
#include "tree/intersect_handler.h"

namespace utilities {

QuadTreeNode::QuadTreeNode(Rect rect, size_t depth) : rect_(rect), depth_(depth) {}

auto QuadTreeNode::IsLeafNode() const -> bool { return this->node_type_ == QuadTreeNodeType::LEAF_NODE; }

void QuadTreeNode::SetNodeType(QuadTreeNodeType node_type) { this->node_type_ = node_type; }

auto QuadTreeNode::IsContain(const Shape2D &obj) const -> bool { return rect_.Interact(obj); }

}  // namespace utilities
