#include <gtest/gtest.h>
#include <cmath>
#include <vector>

namespace utilities {

auto Distance(const std::vector<double> &lhs, const std::vector<double> &rhs) -> double {
  EXPECT_EQ(lhs.size(), rhs.size());

  int n = lhs.size();
  double sum = 0;
  for (int i = 0; i < n; ++i) {
    double diff = lhs[i] - rhs[i];
    sum += diff * diff;
  }
  return std::sqrt(sum);
}

}  // namespace utilities