#pragma once

#include <atomic>
#include <thread>


class TicketLock {
public:
  TicketLock() : next_ticket_(0), now_serving_(0) {
  }

  void Lock() {
      size_t my_ticket = next_ticket_.fetch_add(1);
      while (now_serving_ != my_ticket) {
          std::this_thread::yield();
      }

  }

  void Unlock() {
      now_serving_.fetch_add(1);
  }

private:
    std::atomic<size_t> next_ticket_, now_serving_;
};

