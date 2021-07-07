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
    : id_(id), second_fork_(left_fork), first_fork_(right_fork) {
      if (second_fork_->Id() > first_fork_->Id()) {
          std::swap(second_fork_, first_fork_);
      }
  }

  size_t Id() const {
      return id_;
  }

  void Eat() {
      first_fork_->Get();
      second_fork_->Get();
  }

  void Think() {
      first_fork_->Put();
      second_fork_->Put();
  }

 private:
    const size_t id_;
    Fork* second_fork_;
    Fork* first_fork_;
};

