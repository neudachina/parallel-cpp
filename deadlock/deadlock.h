#pragma once
#include <thread>
#include <mutex>
#include <chrono>


class Deadlock {
 public:
  Deadlock() : interval_(100) {
  }

  void ThreadOne() {
      std::unique_lock lock(first_mutex_);
      std::this_thread::sleep_for(interval_);
      while (!second_mutex_.try_lock()) {
          std::this_thread::sleep_for(interval_);
      }
      second_mutex_.unlock();
  }

  void ThreadTwo() {
      std::unique_lock lock(second_mutex_);
      while (!first_mutex_.try_lock()) {
          std::this_thread::sleep_for(interval_);
      }
      first_mutex_.unlock();
  }

 private:
    std::mutex first_mutex_, second_mutex_;
    std::chrono::milliseconds interval_;
};

