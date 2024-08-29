#include "tree/node/kd_tree_node.h"

#include <cmath>
#include <iostream>
#include <sstream>

namespace utilities {

KDTreeNode::KDTreeNode(int dimension, int div, std::vector<double> &data)
    : dimension_(dimension), div_(div), data_(data) {}

auto KDTreeNode::Distance(const std::vector<double> &target) -> double {
  double ret = 0;
  for (unsigned int i = 0; i < dimension_; ++i) {
    double diff = data_[i] - target[i];
    ret += diff * diff;
  }
  return std::sqrt(ret);
}

void KDTreeNode::Print() {
  std::stringstream ss;
  ss << "(";
  int n = data_.size();
  for (int i = 0; i < n; ++i) {
    ss << data_[i] << (i == n - 1 ? "" : ", ");
  }
  ss << ")";
  std::cout << ss.str() << ", div: " << div_ << std::endl;
}

}  // namespace utilities