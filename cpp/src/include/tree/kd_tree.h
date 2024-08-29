#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "tree/node/kd_tree_node.h"

namespace utilities {
struct Cmp {
  auto operator()(const std::pair<double, std::vector<double> *> &lhs,
                  const std::pair<double, std::vector<double> *> &rhs) -> bool {
    return lhs.first < rhs.first;
  }
};

class KDTree {
 public:
  KDTree() = delete;
  KDTree(const KDTree &other) = delete;

  explicit KDTree(int dimension, std::vector<std::vector<double>> &&data);

  void Build();

  void Query(const std::vector<double> &target, unsigned int num, std::vector<std::vector<double>> &result);

  void PrintTree();

 private:
  auto Build(int div, std::vector<std::vector<double>>::iterator begin, std::vector<std::vector<double>>::iterator end)
      -> std::shared_ptr<KDTreeNode>;

  void Query(const std::shared_ptr<KDTreeNode> &node, const std::vector<double> &target, unsigned int num);

  unsigned int dimension_;
  std::vector<std::vector<double>> data_;
  std::shared_ptr<KDTreeNode> root_{nullptr};
  std::priority_queue<std::pair<double, std::vector<double> *>, std::vector<std::pair<double, std::vector<double> *>>,
                      Cmp>
      pq_;
};

}  // namespace utilities