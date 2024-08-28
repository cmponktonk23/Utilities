#include <sstream>

#include "tree/inside_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"

namespace utilities {
template <typename Geometry2DType, typename InsideHandler>
QuadTreeInternalNode<Geometry2DType, InsideHandler>::QuadTreeInternalNode(Rect rect,
                                                                          const InsideHandler &inside_handler,
                                                                          size_t depth)
    : QuadTreeNode<Geometry2DType, InsideHandler>(rect, inside_handler, depth) {
  this->SetNodeType(QuadTreeNodeType::INTERNAL_NODE);
  for (auto &child : children_) {
    child = nullptr;
  }
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTreeInternalNode<Geometry2DType, InsideHandler>::Print() {
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

template class QuadTreeInternalNode<Rect, RectInsideRectHandler>;

}  // namespace utilities
