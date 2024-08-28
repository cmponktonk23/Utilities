#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

#include "common/geometries/rect.h"
#include "tree/node/quad_tree_node.h"

namespace utilities {

class QuadTree {
 public:
  QuadTree() = delete;
  QuadTree(const QuadTree &other) = delete;

  explicit QuadTree(size_t max_children, size_t max_depth, const Rect &rect);

  auto IsEmpty() const -> bool;

  void Build(const std::vector<Shape2D> &objects);

  auto Insert(const Shape2D &obj) -> bool;

  void Query(const Shape2D &obj, std::unordered_set<size_t> &candidates) const;

  void PrintTree();

  void Traverse(std::function<void(const Rect &)>);

 private:
  auto Build(size_t depth, Rect rect, const std::vector<Shape2D> &objects) -> std::unique_ptr<QuadTreeNode>;

  void Insert(size_t depth, std::unique_ptr<QuadTreeNode> &node, const Shape2D &obj);

  void Query(size_t depth, const std::unique_ptr<QuadTreeNode> &node, const Shape2D &obj,
             std::unordered_set<size_t> &candidates) const;

  void SplitRect(const Rect &rect, Rect rects[4]) const;

  void SplitObjectsByRects(const std::vector<Shape2D> &objects, std::vector<Shape2D> split_objects[4], Rect rects[4]);

  const size_t max_children_;
  const size_t max_depth_;
  std::unique_ptr<QuadTreeNode> root_{nullptr};
  Rect rect_;
};

}  // namespace utilities
