#include <sstream>

#include "tree/intersect_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"

namespace utilities {

QuadTreeInternalNode::QuadTreeInternalNode(Rect rect, size_t depth) : QuadTreeNode(rect, depth) {
  this->SetNodeType(QuadTreeNodeType::INTERNAL_NODE);
  for (auto &child : children_) {
    child = nullptr;
  }
}

void QuadTreeInternalNode::Print() {
  std::stringstream ss;
  ss << "Children[";
  for (int i = 0; i < 4; ++i) {
    auto &child = children_[i];
    if (child) {
      ss << "1";
    } else {
      ss << "0";
    }
    if (i < 3) {
      ss << ",";
    }
  }
  ss << "]";

  std::cout << "InternalNode[" << this->rect_ << ", " << ss.str() << "]"
            << " ";
}

}  // namespace utilities
