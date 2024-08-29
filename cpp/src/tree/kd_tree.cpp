#include "tree/kd_tree.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include "common/marcos.h"

namespace utilities {

KDTree::KDTree(int dimension, std::vector<std::vector<double>> &&data)
    : dimension_(dimension), data_(std::move(data)) {}

void KDTree::Build() { root_ = Build(0, data_.begin(), data_.end()); }

auto KDTree::Build(int div, std::vector<std::vector<double>>::iterator begin,
                   std::vector<std::vector<double>>::iterator end) -> std::shared_ptr<KDTreeNode> {
  if (begin >= end) {
    return nullptr;
  }

  auto mid = std::next(begin, std::distance(begin, end) >> 1);
  std::nth_element(begin, mid, end,
                   [&](const std::vector<double> &lhs, const std::vector<double> &rhs) { return lhs[div] < rhs[div]; });
  auto node = std::make_shared<KDTreeNode>(dimension_, div, *mid);

  if (begin + 1 == end) {
    return node;
  }

  int next_div = (div + 1) % dimension_;
  node->left_ = Build(next_div, begin, mid);
  node->right_ = Build(next_div, mid + 1, end);
  return node;
}

void KDTree::Query(const std::vector<double> &target, unsigned int num, std::vector<std::vector<double>> &result) {
  if (!root_) {
    return;
  }

  MSG_ASSERT(target.size() == dimension_, "Dimension mismatch");

  Query(root_, target, num);

  while (!pq_.empty()) {
    result.push_back(*pq_.top().second);
    pq_.pop();
  }

  std::reverse(result.begin(), result.end());
}

void KDTree::Query(const std::shared_ptr<KDTreeNode> &node, const std::vector<double> &target, unsigned int num) {
  if (!node) {
    return;
  }

  double distance = node->Distance(target);
  if (pq_.size() < num) {
    pq_.push(std::make_pair(distance, &node->data_));
  } else if (pq_.top().first > distance) {
    pq_.push(std::make_pair(distance, &node->data_));
    pq_.pop();
  }

  if (!node->left_ && !node->right_) {
    return;
  }

  int div = node->div_;
  double div_distance = target[div] - node->data_[div];

  auto one_way = node->left_;
  auto another_way = node->right_;

  if (div_distance > 0) {
    swap(one_way, another_way);
  }

  Query(one_way, target, num);
  if (pq_.top().first > div_distance || pq_.size() < num) {
    Query(another_way, target, num);
  }
}

void KDTree::PrintTree() {
  std::vector<std::vector<KDTreeNode *>> layers;
  std::queue<std::pair<KDTreeNode *, unsigned int>> q;
  q.push({root_.get(), 1});
  unsigned int last_layer = 0;

  while (!q.empty()) {
    auto [node, layer] = q.front();
    q.pop();

    if (layer > last_layer) {
      layers.emplace_back(std::vector<KDTreeNode *>());
      last_layer = layer;
    }
    layers.back().push_back(node);

    if (node->left_) {
      q.push({node->left_.get(), layer + 1});
    }
    if (node->right_) {
      q.push({node->right_.get(), layer + 1});
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