#include <gtest/gtest.h>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <random>

#include "test_utils.h"
#include "tree/kd_tree.h"

namespace utilities {

auto GetRandomDouble(double min, double max) -> double {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(min, max);

  return dis(gen);
}

void BruteForceKNN(const std::vector<std::vector<double>> &data, std::vector<double> &target, unsigned int num,
                   std::vector<std::vector<double>> &result) {
  std::priority_queue<std::pair<double, int>> pq;
  int n = data.size();
  for (int i = 0; i < n; ++i) {
    double d = Distance(data[i], target);
    if (pq.size() < num) {
      pq.push({d, i});
    } else if (d < pq.top().first) {
      pq.push({d, i});
      pq.pop();
    }
  }

  while (!pq.empty()) {
    result.push_back(data[pq.top().second]);
    pq.pop();
  }
}

TEST(KDTreeTests, BasicTest) {
  std::vector<std::vector<double>> data = {{0, 2}, {-11, 3.7}, {10000, 10000001}, {-99, -0.15}};
  //   std::vector<std::vector<double>> data = {{1, 1}, {1, 3}, {3, 4}};
  std::vector<double> target = {11, 5.3};
  //   std::vector<double> target = {2, 3};

  auto data1 = data;
  KDTree tree(2, std::move(data1));
  tree.Build();

  std::vector<std::vector<double>> result1;
  tree.Query(target, 2, result1);

  //   tree.PrintTree();

  EXPECT_EQ(result1.size(), 2);
  //   std::cout << result.size() << std::endl;

  std::vector<std::vector<double>> result2;
  BruteForceKNN(data, target, 2, result2);

  std::sort(result1.begin(), result1.end());
  std::sort(result2.begin(), result2.end());

  EXPECT_EQ(result1, result2);
}

TEST(KDTreeTests, BasicTest2) {
  std::vector<std::vector<double>> data = {{-652131, 243699}, {773884, -609616}, {-898772, 327745}, {-347602, -727484},
                                           {601347, -291509}, {-482331, 350763}, {399189, 570512},  {103717, -70241.1},
                                           {-477011, 577673}, {847091, -17587.5}};
  //   std::vector<std::vector<double>> data = {{1, 1}, {1, 3}, {3, 4}};
  std::vector<double> target = {-100064, -771560};
  //   std::vector<double> target = {2, 3};

  auto data1 = data;
  KDTree tree(2, std::move(data1));
  tree.Build();

  std::vector<std::vector<double>> result1;
  tree.Query(target, 3, result1);

  //   tree.PrintTree();

  EXPECT_EQ(result1.size(), 3);
  //   std::cout << result.size() << std::endl;

  std::vector<std::vector<double>> result2;
  BruteForceKNN(data, target, 3, result2);

  std::sort(result1.begin(), result1.end());
  std::sort(result2.begin(), result2.end());

  EXPECT_EQ(result1, result2);
}

TEST(KDTreeTests, BasicTestRandom) {
  unsigned int n = 10000;
  unsigned int k = 5;
  unsigned int num = 100;
  double min = -1000000;
  double max = 1000000;

  std::vector<std::vector<double>> data(n);
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < k; ++j) {
      data[i].push_back(GetRandomDouble(min, max));
    }
  }

  //   for (unsigned int i = 0; i < n; ++i) {
  //     std::cout << "{";
  //     for (unsigned int j = 0; j < k; ++j) {
  //       std::cout << data[i][j] << (j == k - 1 ? "" : ", ");
  //     }
  //     std::cout << "},";
  //   }
  //   std::cout << std::endl;

  std::vector<double> target;
  for (unsigned int j = 0; j < k; ++j) {
    target.push_back(GetRandomDouble(min, max));
  }

  //   std::cout << "{";
  //   for (unsigned int j = 0; j < k; ++j) {
  //     std::cout << target[j] << (j == k - 1 ? "" : ", ");
  //   }
  //   std::cout << "}";

  auto start = std::chrono::high_resolution_clock::now();

  auto data1 = data;
  KDTree tree(k, std::move(data1));
  tree.Build();
  std::vector<std::vector<double>> result1;
  tree.Query(target, num, result1);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  std::cout << "KDTree Elapsed time: " << elapsed.count() << " ms" << std::endl;

  start = std::chrono::high_resolution_clock::now();

  std::vector<std::vector<double>> result2;
  BruteForceKNN(data, target, num, result2);

  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  std::cout << "BruteForceKNN Elapsed time: " << elapsed.count() << " ms" << std::endl;

  std::sort(result1.begin(), result1.end());
  std::sort(result2.begin(), result2.end());

  //   for (auto &p : result1) {
  //     std::cout << std::fixed << std::setprecision(4) << Distance(target, p) << " ";
  //   }
  //   std::cout << std::endl;

  //   for (auto &p : result2) {
  //     std::cout << std::fixed << std::setprecision(4) << Distance(target, p) << " ";
  //   }
  //   std::cout << std::endl;

  EXPECT_EQ(result1, result2);
}

const int K = 5;
int top;

struct Point {
  double coords[K];
  double dis;
  int div;
  bool operator<(const Point &other) const { return dis < other.dis; }
};

inline double Distance(const Point &lhs, const Point &rhs) {
  double ret = 0;
  for (int i = 0; i < K; ++i) {
    double diff = lhs.coords[i] - rhs.coords[i];
    ret += diff * diff;
  }
  return std::sqrt(ret);
}

inline void Build(int l, int r, Point p[], int div) {
  if (l > r) {
    return;
  }

  int mid = (l + r) >> 1;
  std::nth_element(p + l, p + mid, p + r + 1,
                   [&](const Point &lhs, const Point &rhs) { return lhs.coords[div] < rhs.coords[div]; });
  p[mid].div = div;

  if (l == r) {
    return;
  }

  int next_div = (div + 1) % (K - 1);
  Build(l, mid - 1, p, next_div);
  Build(mid + 1, r, p, next_div);
}

inline void Query(int l, int r, Point &target, Point p[], std::priority_queue<Point> &pq, unsigned int num) {
  if (l > r) {
    return;
  }

  int mid = (l + r) >> 1;
  double d = Distance(p[mid], target);
  p[mid].dis = d;

  auto &pp = p[mid];
  if (pq.size() < num) {
    pq.push(pp);
  } else if (top > d) {
    pq.push(pp);
    pq.pop();
  }
  top = pq.top().dis;

  if (l == r) {
    return;
  }

  double dd = target.coords[pp.div] - p[mid].coords[pp.div];
  if (dd <= 0) {
    Query(l, mid - 1, target, p, pq, num);
    if (top > dd || pq.size() < num) {
      Query(mid + 1, r, target, p, pq, num);
    }
  } else {
    Query(mid + 1, r, target, p, pq, num);
    if (top > dd || pq.size() < num) {
      Query(l, mid - 1, target, p, pq, num);
    }
  }
}

TEST(KDTreeTests, BasicTestRandomMultiple) {
  unsigned int q = 10;
  unsigned int n = 50000;
  unsigned int k = 5;
  double min = -1000000;
  double max = 1000000;

  for (int ii = 0; ii < 10; ++ii) {
    std::vector<std::vector<double>> data(n);
    Point points[n];
    for (unsigned int i = 0; i < n; ++i) {
      for (unsigned int j = 0; j < k; ++j) {
        data[i].push_back(GetRandomDouble(min, max));
        points[i].coords[j] = data[i][j];
      }
    }

    std::vector<unsigned int> nums(q);
    std::vector<std::vector<double>> targets(q);
    std::vector<std::vector<std::vector<double>>> result1(q);
    std::vector<std::vector<std::vector<double>>> result2(q);
    std::vector<std::vector<std::vector<double>>> result3(q);
    std::priority_queue<Point> pq;

    Point ptargets[q];

    for (unsigned int i = 0; i < q; ++i) {
      nums[i] = static_cast<unsigned int>(GetRandomDouble(1, 10));

      for (unsigned int j = 0; j < k; ++j) {
        targets[i].push_back(GetRandomDouble(min, max));
        ptargets[i].coords[j] = targets[i][j];
      }
    }

    //
    // Cpp version QuadTree
    //
    auto start = std::chrono::high_resolution_clock::now();

    auto data1 = data;
    KDTree tree(k, std::move(data1));
    tree.Build();

    for (unsigned int i = 0; i < q; ++i) {
      tree.Query(targets[i], nums[i], result1[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "KDTree Elapsed time: " << elapsed.count() << " ms" << std::endl;

    //
    // C version QuadTree
    //
    start = std::chrono::high_resolution_clock::now();

    Build(0, n - 1, points, 0);

    for (unsigned int i = 0; i < q; ++i) {
      Query(0, n - 1, ptargets[i], points, pq, nums[i]);

      // std::cout << pq.size() << std::endl;
      while (!pq.empty()) {
        auto arr = pq.top().coords;
        result3[i].emplace_back(arr, arr + K);
        pq.pop();
      }
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "KDTree C Version Elapsed time: " << elapsed.count() << " ms" << std::endl;

    //
    // Brute force
    //
    start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < q; ++i) {
      BruteForceKNN(data, targets[i], nums[i], result2[i]);
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "BruteForceKNN Elapsed time: " << elapsed.count() << " ms" << std::endl;

    for (unsigned int i = 0; i < q; ++i) {
      std::sort(result1[i].begin(), result1[i].end());
      std::sort(result2[i].begin(), result2[i].end());
      std::sort(result3[i].begin(), result3[i].end());

      int nn = result1[i].size();
      std::cout << std::endl;
      for (int j = 0; j < nn; ++j) {
        std::cout << Distance(result1[i][j], targets[i]) << " ";
      }
      std::cout << std::endl;
      for (int j = 0; j < nn; ++j) {
        std::cout << Distance(result2[i][j], targets[i]) << " ";
      }
      std::cout << std::endl;
      for (int j = 0; j < nn; ++j) {
        std::cout << Distance(result3[i][j], targets[i]) << " ";
      }
      std::cout << std::endl;

      EXPECT_EQ(result1[i], result2[i]);
      EXPECT_EQ(result1[i], result3[i]);
    }
  }
}

}  // namespace utilities
