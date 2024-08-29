#pragma once

#include <memory>
#include <queue>
#include <vector>

namespace utilities {

class KDTreeNode {
  friend class KDTree;

 public:
  KDTreeNode() = delete;
  KDTreeNode(const KDTreeNode &other) = delete;

  explicit KDTreeNode(int dimension, int div, std::vector<double> &data);

  auto Distance(const std::vector<double> &target) -> double;

  void Print();

 private:
  unsigned int dimension_;
  int div_;
  std::vector<double> &data_;

  std::shared_ptr<KDTreeNode> left_{nullptr};
  std::shared_ptr<KDTreeNode> right_{nullptr};
};

}  // namespace utilities