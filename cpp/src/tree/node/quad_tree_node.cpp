#include "tree/node/quad_tree_node.h"
#include "tree/inside_handler.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
QuadTreeNode<Geometry2DType, InsideHandler>::QuadTreeNode(Rect rect, const InsideHandler &inside_handler)
    : rect_(rect), inside_handler_(inside_handler) {}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTreeNode<Geometry2DType, InsideHandler>::IsLeafNode() const -> bool {
  return this->node_type_ == QuadTreeNodeType::LEAF_NODE;
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTreeNode<Geometry2DType, InsideHandler>::SetNodeType(QuadTreeNodeType node_type) {
  this->node_type_ = node_type;
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTreeNode<Geometry2DType, InsideHandler>::IsContain(Geometry2DType obj) const -> bool {
  return this->inside_handler_(obj, rect_);
}

template class QuadTreeNode<Rect, RectInsideRectHandler>;

}  // namespace utilities
