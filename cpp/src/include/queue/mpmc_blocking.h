#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace utilities {

template <class T>
class MPMCBlockingQueue {
 public:
  MPMCBlockingQueue() = default;
  ~MPMCBlockingQueue() = default;

  auto Enqueue(T element) -> void;
  auto Dequeue() -> T;

 private:
  std::mutex m_;
  std::condition_variable cv_;
  std::queue<T> q_;
};

}  // namespace utilities
