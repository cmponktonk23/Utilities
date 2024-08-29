#include <gtest/gtest.h>

#include "test_utils.h"
#include "tree/node/kd_tree_node.h"

namespace utilities {

TEST(KDTreeNodeTests, TestDistance) {
  std::vector<double> data = {{0, 2}};
  std::vector<double> target = {3, 7};

  KDTreeNode node(2, 0, data);

  EXPECT_EQ(node.Distance(target), Distance(data, target));
}

}  // namespace utilities
