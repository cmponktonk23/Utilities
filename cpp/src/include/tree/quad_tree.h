#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "common/types/rect.h"
#include "tree/node/quad_tree_node.h"

namespace utilities {

template <typename Geometry2DType, typename InsideHandler>
class QuadTree {
 public:
  QuadTree() = delete;
  QuadTree(const QuadTree &other) = delete;

  explicit QuadTree(size_t max_children, size_t max_depth, const Rect &rect, const InsideHandler &inside_handler);

  auto IsEmpty() const -> bool;

  void Build(const std::vector<Geometry2DType> &objects);

  auto Insert(const Geometry2DType &obj) -> bool;

  void PrintTree();

 private:
  auto Build(size_t depth, Rect rect, const std::vector<Geometry2DType> &objects)
      -> std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>>;

  void Insert(size_t depth, std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>> &node,
              const Geometry2DType &obj);

  void SplitRect(const Rect &rect, Rect rects[4]);

  void SplitObjectsByRects(const std::vector<Geometry2DType> &objects, std::vector<Geometry2DType> split_objects[4],
                           Rect rects[4]);

  const size_t max_children_;
  const size_t max_depth_;
  std::unique_ptr<QuadTreeNode<Geometry2DType, InsideHandler>> root_{nullptr};
  Rect rect_;
  InsideHandler inside_handler_;
};

}  // namespace utilities
