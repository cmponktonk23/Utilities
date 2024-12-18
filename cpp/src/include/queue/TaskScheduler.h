#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

#include "queue/mpmc_blocking.h"

namespace utilities {

struct Task {
 public:
  int id_;
};

class TaskScheduler {
 public:
  explicit TaskScheduler();
  explicit TaskScheduler(int consumer_cnt);
  ~TaskScheduler();

  auto Schedule() -> void;

  auto StartConsumerThread() -> void;

 private:
  const int consumer_cnt_;
  std::vector<std::thread> consumers_;
  MPMCBlockingQueue<std::optional<Task>> queue_;
};

}  // namespace utilities
