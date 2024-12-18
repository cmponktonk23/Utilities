#include "queue/mpmc_blocking.h"

namespace utilities {
template <class T>
auto MPMCBlockingQueue<T>::Enqueue(T element) -> void {
  std::unique_lock<std::mutex> lk(m_);
  q_.push(std::move(element));
  lk.unlock();
  cv_.notify_one();
}

template <class T>
auto MPMCBlockingQueue<T>::Dequeue() -> T {
  std::unique_lock<std::mutex> lk(m_);
  cv_.wait(lk, [&]() { return !q_.empty(); });
  T element = std::move(q_.front());
  q_.pop();
  return element;
}
}  // namespace utilities