#include "queue/TaskScheduler.h"

namespace utilities {

TaskScheduler::TaskScheduler() : TaskScheduler(1) {}

TaskScheduler::TaskScheduler(int consumer_cnt) : consumer_cnt_(consumer_cnt) {
  for (int i = 0; i < consumer_cnt_; ++i) {
    consumers_.emplace_back([&] { StartConsumerThread(); });
  }
}

TaskScheduler::~TaskScheduler() {
  for (int i = 0; i < consumer_cnt_; ++i) {
    queue_.Enqueue(std::nullopt);
  }

  for (int i = 0; i < consumer_cnt_; ++i) {
    consumers_[i].join();
  }
}

}  // namespace utilities