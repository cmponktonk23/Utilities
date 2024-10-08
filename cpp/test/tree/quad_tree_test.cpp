#include <gtest/gtest.h>

#include "common/geometries/rect.h"
#include "tree/intersect_handler.h"
#include "tree/quad_tree.h"

namespace utilities {
TEST(QuadTreeTests, TestSingleNode) {
  Rect area(Vector2D(0, 0), Vector2D(16, 16));

  QuadTree tree(4, 4, area);

  ASSERT_TRUE(tree.IsEmpty());

  auto objects = std::vector<Rect>{
      Rect(Vector2D(0, 0), Vector2D(1, 1)), Rect(Vector2D(0, 0), Vector2D(2, 2)), Rect(Vector2D(0, 0), Vector2D(3, 3)),
      Rect(Vector2D(0, 0), Vector2D(5, 5)), Rect(Vector2D(0, 0), Vector2D(6, 6)),
  };

  tree.Build(objects);

  tree.PrintTree();
}
}  // namespace utilities
