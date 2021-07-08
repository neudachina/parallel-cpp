#pragma once

#include <atomic>
#include <thread>

struct Node {
    bool flag = false;
    Node* next = nullptr;
};

class MCSLock {
 public:
  MCSLock() : tail_(nullptr) {}

  void Lock() {
      Node* prev = tail_.exchange(&node_);
      if (prev) {
          prev->next = &node_;
          while (!node_.flag)
              std::this_thread::yield();
      }

  }

  void Unlock() {
      Node* expected = &node_;
      if (tail_.compare_exchange_weak(expected, nullptr)) {
          return;
      }
      while (!node_.next)
          std::this_thread::yield();
      node_.next->flag = true;
  }

private:
    thread_local static Node node_;
    std::atomic<Node*> tail_;
};

