#include <iostream>
#include <queue>

#include "common/marcos.h"
#include "tree/intersect_handler.h"
#include "tree/node/quad_tree_internal_node.h"
#include "tree/node/quad_tree_leaf_node.h"
#include "tree/quad_tree.h"

namespace utilities {

QuadTree::QuadTree(size_t max_children, size_t max_depth, const Rect &rect)
    : max_children_(max_children), max_depth_(max_depth), rect_(rect) {}

void QuadTree::Build(const std::vector<Shape2D> &objects) { this->root_ = this->Build(1, rect_, objects); }

void QuadTree::SplitRect(const Rect &rect, Rect rects[4]) const {
  auto left_down = rect.GetLeftDown();
  auto right_up = rect.GetRightUp();
  auto center = (left_down + right_up) * 0.5;
  auto width = rect.GetWidth();

  rects[0] = Rect(left_down, center);
  rects[1] = Rect(left_down + Vector2D(width * 0.5, 0), center + Vector2D(width * 0.5, 0));
  rects[2] = Rect(center, right_up);
  rects[3] = Rect(center - Vector2D(width * 0.5, 0), right_up - Vector2D(width * 0.5, 0));
}

void QuadTree::SplitObjectsByRects(const std::vector<Shape2D> &objects, std::vector<Shape2D> split_objects[4],
                                   Rect rects[4]) {
  for (int i = 0; i < 4; ++i) {
    split_objects[i].clear();
  }

  for (auto &obj : objects) {
    for (int i = 0; i < 4; ++i) {
      if (obj.Interact(rects[i])) {
        split_objects[i].push_back(obj);
      }
    }
  }
}

auto QuadTree::Build(size_t depth, Rect rect, const std::vector<Shape2D> &objects) -> std::unique_ptr<QuadTreeNode> {
  if (objects.empty()) {
    return nullptr;
  }

  //   std::cout << rect << " " << objects.size() << std::endl;
  if (objects.size() <= max_children_ || depth == max_depth_) {
    auto leaf_node = std::make_unique<QuadTreeLeafNode>(rect, depth);
    leaf_node->objects_ = objects;
    return leaf_node;
  }

  Rect rects[4];
  SplitRect(rect, rects);
  std::vector<Shape2D> split_objects[4];
  SplitObjectsByRects(objects, split_objects, rects);

  auto internal_node = std::make_unique<QuadTreeInternalNode>(rect, depth);

  for (int i = 0; i < 4; i++) {
    internal_node->children_[i] = this->Build(depth + 1, rects[i], split_objects[i]);
  }

  return internal_node;
}

auto QuadTree::IsEmpty() const -> bool { return !this->root_; }

auto QuadTree::Insert(const Shape2D &obj) -> bool {
  if (!obj.Interact(rect_)) {
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

void QuadTree::Insert(size_t depth, std::unique_ptr<QuadTreeNode> &node, const Shape2D &obj) {
  // leaf node
  if (node->IsLeafNode()) {
    auto *leaf_node = dynamic_cast<QuadTreeLeafNode *>(node.get());
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
  auto *internal_node = dynamic_cast<QuadTreeInternalNode *>(node.get());

  for (int i = 0; i < 4; ++i) {
    if (obj.Interact(rects[i])) {
      if (!internal_node->children_[i]) {
        internal_node->children_[i] = std::make_unique<QuadTreeLeafNode>(rects[i], depth);
      }
      Insert(depth + 1, internal_node->children_[i], obj);
    }
  }
}

void QuadTree::Query(const Shape2D &obj, std::unordered_set<size_t> &candidates) const {
  if (!root_) {
    return;
  }
  Query(1, root_, obj, candidates);
}

void QuadTree::Query(size_t depth, const std::unique_ptr<QuadTreeNode> &node, const Shape2D &obj,
                     std::unordered_set<size_t> &candidates) const {
  if (!node) {
    return;
  }

  if (node->IsLeafNode()) {
    auto *leaf_node = dynamic_cast<QuadTreeLeafNode *>(node.get());
    for (auto &obj : leaf_node->objects_) {
      candidates.insert(obj.id_);
    }
    return;
  }

  Rect rects[4];
  SplitRect(node->rect_, rects);

  // internal node
  auto *internal_node = dynamic_cast<QuadTreeInternalNode *>(node.get());

  for (int i = 0; i < 4; ++i) {
    if (obj.Interact(rects[i]) && internal_node->children_[i]) {
      Query(depth + 1, internal_node->children_[i], obj, candidates);
    }
  }
}

void QuadTree::Traverse(std::function<void(const Rect &)> callback) {
  if (IsEmpty()) {
    return;
  }

  std::queue<QuadTreeNode *> q;
  q.push(root_.get());

  while (!q.empty()) {
    auto *node = q.front();
    q.pop();

    callback(node->rect_);

    if (!node->IsLeafNode()) {
      auto *internal_node = dynamic_cast<QuadTreeInternalNode *>(node);
      for (auto &child : internal_node->children_) {
        if (child) {
          q.push(child.get());
        }
      }
    }
  }
}

void QuadTree::PrintTree() {
  std::vector<QuadTreeNode *> layers[max_depth_ + 1];
  std::queue<QuadTreeNode *> q;
  q.push(root_.get());
  while (!q.empty()) {
    auto *node = q.front();
    q.pop();
    layers[node->depth_].push_back(node);
    if (!node->IsLeafNode()) {
      auto *internal_node = dynamic_cast<QuadTreeInternalNode *>(node);
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

}  // namespace utilities
