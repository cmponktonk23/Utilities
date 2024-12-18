#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <thread>
#include <vector>

template <typename T>
class SPSCQueue {
 public:
  SPSCQueue() = default;
  virtual ~SPSCQueue() = default;
  virtual bool enqueue(T &&item) = 0;
  virtual std::optional<T> dequeue() = 0;
};

template <typename T>
class SPSCLockQueue : public SPSCQueue<T> {
 public:
  SPSCLockQueue() = default;
  virtual ~SPSCLockQueue() = default;

  bool enqueue(T &&item) override {
    std::unique_lock<std::mutex> lk(mutex_);
    queue_.push(std::move(item));
    lk.unlock();
    not_empty_cv_.notify_one();
    return true;
  }

  std::optional<T> dequeue() override {
    std::unique_lock<std::mutex> lk(mutex_);
    not_empty_cv_.wait(lk, [&]() { return !queue_.empty(); });
    T item = std::move(queue_.front());
    queue_.pop();
    return item;
  }

 private:
  std::mutex mutex_;
  std::condition_variable not_empty_cv_;
  std::queue<T> queue_;
};

template <typename T>
class SPSCLockFreeQueue : public SPSCQueue<T> {
 public:
  explicit SPSCLockFreeQueue(size_t capacity) : buffer_(capacity), head_(0), tail_(0), capacity_(capacity) {}

  virtual ~SPSCLockFreeQueue() = default;

  bool enqueue(T &&item) override {
    size_t current_tail = tail_.load(std::memory_order_relaxed);
    size_t next_tail = increment(current_tail);
    if (next_tail != head_.load(std::memory_order_relaxed)) {
      buffer_[current_tail] = std::move(item);
      tail_.store(next_tail, std::memory_order_relaxed);
      return true;
    }
    return false;
  }

  std::optional<T> dequeue() override {
    size_t current_head = head_.load(std::memory_order_relaxed);
    if (current_head == tail_.load(std::memory_order_relaxed)) {
      return std::nullopt;
    }
    T item = std::move(buffer_[current_head]);
    head_.store(increment(current_head), std::memory_order_relaxed);
    return item;
  }

 private:
  //   size_t increment(size_t idx) const { return (idx + 1) % capacity_; }
  size_t increment(size_t idx) const { return (idx + 1) & (capacity_ - 1); }

  std::vector<T> buffer_;
  alignas(64) std::atomic<size_t> head_;
  alignas(64) std::atomic<size_t> tail_;
  const size_t capacity_;
};

template <typename T>
void shuffle_vector(std::vector<T> &vec) {
  std::random_device rd;
  std::mt19937 gen(rd());

  for (size_t i = vec.size() - 1; i > 0; --i) {
    std::uniform_int_distribution<size_t> dis(0, i);
    size_t j = dis(gen);
    std::swap(vec[i], vec[j]);
  }
}

void test(SPSCQueue<int> &queue) {
  const size_t total_iterations = 10000000;
  std::vector<int> test_data(total_iterations);
  for (int i = 0; i < total_iterations; ++i) {
    test_data[i] = i;
  }
  shuffle_vector(test_data);

  auto start_time = std::chrono::high_resolution_clock::now();

  std::thread producer([&]() {
    for (size_t i = 0; i < total_iterations; ++i) {
      while (!queue.enqueue(static_cast<int>(test_data[i]))) {
        std::this_thread::yield();
      }
    }
  });

  std::thread consumer([&]() {
    int idx = 0;
    while (idx < total_iterations) {
      auto item_opt = queue.dequeue();
      if (item_opt.has_value()) {
        int value = item_opt.value();
        if (value != static_cast<int>(test_data[idx])) {
          std::cerr << "数据不一致！期望值：" << test_data[idx] << "，实际值：" << value << "\n";
          return -1;
        }
        ++idx;
      } else {
        std::this_thread::yield();
      }
    }
    return 0;
  });

  producer.join();
  consumer.join();

  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

  std::cout << "总耗时: " << duration << " 毫秒\n";
  std::cout << "吞吐量: " << (total_iterations * 1000.0 / duration) << " 次操作/秒\n";
}

int main() {
  const size_t queue_capacity = 512;
  SPSCLockFreeQueue<int> lockfree_queue(queue_capacity);
  SPSCLockQueue<int> lock_queue;

  for (int i = 0; i < 30; ++i) {
    test(lockfree_queue);
  }
  //   test(lock_queue);

  return 0;
}