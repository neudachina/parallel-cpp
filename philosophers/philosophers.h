#pragma once

#include <mutex>
#include <thread>


class Fork {
 public:
  Fork(size_t id) : id_(id) {
  }

  size_t Id() const {
    return id_;
  }

  void Get() {
    mutex_.lock();
  }

  bool TryGet() {
    return mutex_.try_lock();
  }

  void Put() {
    mutex_.unlock();
  }

 private:
  size_t id_;
  std::mutex mutex_;
};

class Philosopher {
 public:
  Philosopher(size_t id, Fork* left_fork, Fork* right_fork)
    : id_(id), left_fork_(left_fork), right_fork_(right_fork) {
      if (left_fork_->Id() > right_fork_->Id()) {
          std::swap(left_fork_, right_fork_);
      }
  }

  size_t Id() const {
      return id_;
  }

  void Eat() {
      while (!right_fork_->TryGet());
      while (!left_fork_->TryGet());
  }

  void Think() {
    right_fork_->Put();
    left_fork_->Put();
  }

 private:
  const size_t id_;
  Fork* left_fork_;
  Fork* right_fork_;
};

