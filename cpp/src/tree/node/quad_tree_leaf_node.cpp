#include <iostream>
#include <sstream>

#include "tree/inside_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"

namespace utilities {
template <typename Geometry2DType, typename InsideHandler>
QuadTreeLeafNode<Geometry2DType, InsideHandler>::QuadTreeLeafNode(Rect rect, const InsideHandler &inside_handler,
                                                                  size_t depth)
    : QuadTreeNode<Geometry2DType, InsideHandler>(rect, inside_handler, depth) {
  this->SetNodeType(QuadTreeNodeType::LEAF_NODE);
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTreeLeafNode<Geometry2DType, InsideHandler>::Print() {
  std::cout << "LeafNode[" << this->rect_ << ", " << objects_.size() << "]"
            << " ";
}

template class QuadTreeLeafNode<Rect, RectInsideRectHandler>;

}  // namespace utilities
