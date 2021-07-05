#pragma once
#include <thread>
#include <mutex>
#include <chrono>


class Deadlock {
 public:
  Deadlock() : interval(100) {
  }

  void ThreadOne() {
      std::unique_lock lock(first_mutex);
      std::this_thread::sleep_for(interval);
      while (!second_mutex.try_lock()) {
          std::this_thread::sleep_for(interval);
      }
      second_mutex.unlock();
  }

  void ThreadTwo() {
      std::unique_lock lock(second_mutex);
      while (!first_mutex.try_lock()) {
          std::this_thread::sleep_for(interval);
      }
      first_mutex.unlock();
  }

 private:
    std::mutex first_mutex, second_mutex;
    std::chrono::milliseconds interval;
};

