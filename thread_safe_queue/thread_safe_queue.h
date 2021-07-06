#pragma once

#include <optional>
#include <queue>
#include <condition_variable>
#include <mutex>


template <typename T>
class ThreadSafeQueue {
 public:
  ThreadSafeQueue() {
  }

  void Push(const T& value) {
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.push(value);
      cv_.notify_one();
  }

  T Pop() {
      int x;
      std::unique_lock<std::mutex> lock(mutex_);
      while (queue_.empty()) {
          cv_.wait(lock);
      }
      x = queue_.front();
      queue_.pop();
      return x;
  }

  std::optional<T> TryPop() {
      std::unique_lock<std::mutex> lock(mutex_);
      if (queue_.empty()) {
          return std::nullopt;
      }
      auto value = queue_.front();
      queue_.pop();
      return value;
  }


 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

