#include "tree/node/quad_tree_leaf_node.h"
#include <iostream>
#include "tree/inside_handler.h"

namespace utilities {
template <typename Geometry2DType, typename InsideHandler>
QuadTreeLeafNode<Geometry2DType, InsideHandler>::QuadTreeLeafNode(Rect rect, const InsideHandler &inside_handler)
    : QuadTreeNode<Geometry2DType, InsideHandler>(rect, inside_handler) {
  this->SetNodeType(QuadTreeNodeType::LEAF_NODE);
}

template <typename Geometry2DType, typename InsideHandler>
QuadTreeLeafNode<Geometry2DType, InsideHandler>::~QuadTreeLeafNode() {}

template <typename Geometry2DType, typename InsideHandler>
void QuadTreeLeafNode<Geometry2DType, InsideHandler>::Insert(const Geometry2DType &obj) {
  objects_.push_back(obj);
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTreeLeafNode<Geometry2DType, InsideHandler>::Insert(const std::vector<Geometry2DType> &objects) {
  //   objects_.insert(objects_.end(), objects.begin(), objects.end());
  for (auto &obj : objects) {
    std::cout << obj.GetLeftDown().GetX() << std::endl;
    objects_.push_back(obj);
  }
}

template class QuadTreeLeafNode<Rect, RectInsideRectHandler>;

}  // namespace utilities