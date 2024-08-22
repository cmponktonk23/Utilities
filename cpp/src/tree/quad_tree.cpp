#include "tree/quad_tree.h"
#include <iostream>
#include "tree/inside_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
QuadTree<Geometry2DType, InsideHandler>::QuadTree(const InsideHandler &inside_handler)
    : inside_handler_(std::move(inside_handler)) {}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::Build(Rect rect, const std::vector<Geometry2DType> &objects) {
  this->rect_ = rect;
  this->root_ = this->Build(1, rect, objects);
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTree<Geometry2DType, InsideHandler>::Build(int depth, Rect rect, const std::vector<Geometry2DType> &objects)
    -> std::optional<std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>>> {
  if (objects.empty()) {
    return std::nullopt;
  }

  if (objects.size() < 4 || depth == MAX_DEPTH) {
    // std::cout << "dfalsdflasdfasdfasdf dfasdfasdfasdf asdfasdfasdfsdf" << std::endl;
    auto leaf_node = std::make_unique<QuadTreeLeafNode<Geometry2DType, InsideHandler>>(rect, this->inside_handler_);
    leaf_node->Insert(objects);
    return leaf_node;
  }

  auto left_down = rect.GetLeftDown();
  auto right_up = rect.GetRightUp();
  auto center = (left_down + right_up) * 0.5;
  auto width = rect.GetWidth();

  Rect rects[MAX_CHILDREN] = {
      Rect(left_down, center),
      Rect(left_down + Vector2D(width * 0.5, 0), center + Vector2D(width * 0.5, 0)),
      Rect(center, right_up),
      Rect(center - Vector2D(width * 0.5, 0), right_up - Vector2D(width * 0.5, 0)),
  };

  for (int i = 0; i < MAX_CHILDREN; i++) {
    split_objects_[i].clear();
  }

  for (auto &obj : objects) {
    for (int i = 0; i < MAX_CHILDREN; ++i) {
      if (this->inside_handler_(obj, rects[i])) {
        split_objects_[i].push_back(obj);
      }
    }
  }

  auto internal_node =
      std::make_unique<QuadTreeInternalNode<Geometry2DType, InsideHandler>>(rect, this->inside_handler_);

  for (int i = 0; i < MAX_CHILDREN; i++) {
    internal_node->children_[i] = this->Build(depth + 1, rects[i], split_objects_[i]);
  }

  return internal_node;
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTree<Geometry2DType, InsideHandler>::IsEmpty() const -> bool {
  return !this->root_.has_value();
}

template class QuadTree<Rect, RectInsideRectHandler>;

// template class QuadTree<Point, PointInsideRectHandler>;

}  // namespace utilities
