#include "tree/node/quad_tree_internal_node.h"
#include "tree/inside_handler.h"

namespace utilities {
template <typename Geometry2DType, typename InsideHandler>
QuadTreeInternalNode<Geometry2DType, InsideHandler>::QuadTreeInternalNode(Rect rect,
                                                                          const InsideHandler &inside_handler)
    : QuadTreeNode<Geometry2DType, InsideHandler>(rect, inside_handler) {
  this->SetNodeType(QuadTreeNodeType::INTERNAL_NODE);

  for (int i = 0; i < MAX_CHILDREN; i++) {
    children_[i] = nullptr;
  }
}

template <typename Geometry2DType, typename InsideHandler>
QuadTreeInternalNode<Geometry2DType, InsideHandler>::~QuadTreeInternalNode() {}

template class QuadTreeInternalNode<Rect, RectInsideRectHandler>;

}  // namespace utilities