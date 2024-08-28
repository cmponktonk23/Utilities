#include <iostream>
#include <queue>

#include "common/marcos.h"
#include "tree/inside_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"
#include "tree/quad_tree.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
QuadTree<Geometry2DType, InsideHandler>::QuadTree(size_t max_children, size_t max_depth, const Rect &rect,
                                                  const InsideHandler &inside_handler)
    : max_children_(max_children), max_depth_(max_depth), rect_(rect), inside_handler_(std::move(inside_handler)) {}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::Build(const std::vector<Geometry2DType> &objects) {
  this->root_ = this->Build(1, rect_, objects);
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::SplitRect(const Rect &rect, Rect rects[4]) {
  auto left_down = rect.GetLeftDown();
  auto right_up = rect.GetRightUp();
  auto center = (left_down + right_up) * 0.5;
  auto width = rect.GetWidth();

  rects[0] = Rect(left_down, center);
  rects[1] = Rect(left_down + Vector2D(width * 0.5, 0), center + Vector2D(width * 0.5, 0));
  rects[2] = Rect(center, right_up);
  rects[3] = Rect(center - Vector2D(width * 0.5, 0), right_up - Vector2D(width * 0.5, 0));
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::SplitObjectsByRects(const std::vector<Geometry2DType> &objects,
                                                                  std::vector<Geometry2DType> split_objects[4],
                                                                  Rect rects[4]) {
  for (int i = 0; i < 4; ++i) {
    split_objects[i].clear();
  }

  for (auto &obj : objects) {
    for (int i = 0; i < 4; ++i) {
      if (inside_handler_(obj, rects[i])) {
        split_objects[i].push_back(obj);
      }
    }
  }
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTree<Geometry2DType, InsideHandler>::Build(size_t depth, Rect rect, const std::vector<Geometry2DType> &objects)
    -> std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>> {
  if (objects.empty()) {
    return nullptr;
  }

  //   std::cout << rect << " " << objects.size() << std::endl;
  if (objects.size() <= max_children_ || depth == max_depth_) {
    auto leaf_node =
        std::make_unique<QuadTreeLeafNode<Geometry2DType, InsideHandler>>(rect, this->inside_handler_, depth);
    leaf_node->objects_ = objects;
    return leaf_node;
  }

  Rect rects[4];
  SplitRect(rect, rects);
  std::vector<Geometry2DType> split_objects[4];
  SplitObjectsByRects(objects, split_objects, rects);

  auto internal_node =
      std::make_unique<QuadTreeInternalNode<Geometry2DType, InsideHandler>>(rect, this->inside_handler_, depth);

  std::cout << depth << " " << objects.size() << std::endl;
  for (int i = 0; i < 4; i++) {
    std::cout << rects[i] << " " << split_objects[i].size() << ", ";
  }
  std::cout << std::endl;

  for (int i = 0; i < 4; i++) {
    internal_node->children_[i] = this->Build(depth + 1, rects[i], split_objects[i]);
  }

  return internal_node;
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTree<Geometry2DType, InsideHandler>::IsEmpty() const -> bool {
  return !this->root_;
}

template <typename Geometry2DType, typename InsideHandler>
auto QuadTree<Geometry2DType, InsideHandler>::Insert(const Geometry2DType &obj) -> bool {
  if (!inside_handler_(obj, rect_)) {
    return false;
  }

  // empty tree
  if (root_ == nullptr) {
    root_ = Build(1, rect_, {obj});
    return true;
  }

  Insert(1, root_, obj);
  return true;
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::Insert(size_t depth,
                                                     std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>> &node,
                                                     const Geometry2DType &obj) {
  if (depth == max_depth_) {
    // must be a leaf node
    MSG_ASSERT(node->IsLeafNode(), "Not a leaf node");
    auto *leaf_node = dynamic_cast<QuadTreeLeafNode<Geometry2DType, InsideHandler> *>(node.get());
    leaf_node->objects_.push_back(obj);
    return;
  }

  // leaf node
  if (node->IsLeafNode()) {
    auto *leaf_node = dynamic_cast<QuadTreeLeafNode<Geometry2DType, InsideHandler> *>(node.get());
    leaf_node->objects_.push_back(obj);
    // need split
    if (depth < max_depth_ && leaf_node->objects_.size() > max_children_) {
      node = Build(depth, node->rect_, leaf_node->objects_);
    }
    return;
  }

  Rect rects[4];
  SplitRect(node->rect_, rects);

  // internal node
  auto *internal_node = dynamic_cast<QuadTreeInternalNode<Geometry2DType, InsideHandler> *>(node.get());
  for (int i = 0; i < 4; ++i) {
    if (inside_handler_(obj, rects[i])) {
      if (!internal_node->children_[i]) {
        internal_node->children_[i] =
            std::make_unique<QuadTreeLeafNode<Geometry2DType, InsideHandler>>(rects[i], inside_handler_, depth);
      }
      Insert(depth + 1, internal_node->children_[i], obj);
    }
  }
}

template <typename Geometry2DType, typename InsideHandler>
void QuadTree<Geometry2DType, InsideHandler>::PrintTree() {
  std::vector<QuadTreeNode<Geometry2DType, InsideHandler> *> layers[max_depth_ + 1];
  std::queue<QuadTreeNode<Geometry2DType, InsideHandler> *> q;
  q.push(root_.get());
  while (!q.empty()) {
    auto *node = q.front();
    q.pop();
    layers[node->depth_].push_back(node);
    if (!node->IsLeafNode()) {
      auto *internal_node = dynamic_cast<QuadTreeInternalNode<Geometry2DType, InsideHandler> *>(node);
      for (auto &child : internal_node->children_) {
        if (child) {
          q.push(child.get());
        }
      }
    }
  }
  for (auto &layer : layers) {
    for (auto *node : layer) {
      node->Print();
    }
    std::cout << std::endl;
  }
}

template class QuadTree<Rect, RectInsideRectHandler>;

// template class QuadTree<Point, PointInsideRectHandler>;

}  // namespace utilities
