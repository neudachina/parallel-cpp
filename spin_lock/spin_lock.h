#pragma once

#include <atomic>
#include <thread>

class SpinLock {
 public:
  SpinLock() : flag_(false) {
  }

  void Lock() {
      while (flag_.load() || flag_.exchange(true)) {
          std::this_thread::yield();
      }
  }

  void Unlock() {
      flag_.store(false);
  }

 private:
  std::atomic<bool> flag_;
};

