#pragma once
#include <thread>
#include <mutex>
#include <chrono>


class Deadlock {
 public:
  Deadlock() : interval_(100) {
  }

  void ThreadOne() {
      std::unique_lock f_lock(first_mutex_);
      std::this_thread::sleep_for(interval_);
      std::unique_lock s_lock(second_mutex_);
  }

  void ThreadTwo() {
      std::unique_lock s_lock(second_mutex_);
      std::unique_lock f_lock(first_mutex_);
  }

 private:
    std::mutex first_mutex_, second_mutex_;
    std::chrono::milliseconds interval_;
};

