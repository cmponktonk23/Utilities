#include <iostream>
#include <sstream>

#include "tree/inside_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"

namespace utilities {

QuadTreeLeafNode::QuadTreeLeafNode(Rect rect, size_t depth) : QuadTreeNode(rect, depth) {
  this->SetNodeType(QuadTreeNodeType::LEAF_NODE);
}

void QuadTreeLeafNode::Print() {
  std::cout << "LeafNode[" << this->rect_ << ", " << objects_.size() << "]"
            << " ";
}

}  // namespace utilities
