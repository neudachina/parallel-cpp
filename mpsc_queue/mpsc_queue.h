#pragma once

#include <optional>

template <typename T>
class MPSCQueue {
 public:
  MPSCQueue() : head_(nullptr) {}

  ~MPSCQueue() {
      while (head_) {
          Pop();
      }
  }

  void Push(const T& value) {
      auto node = new Node;
      node->value = value;
      while (true) {
          node->next = head_.load();
          auto tmp = node->next;
          if (head_.compare_exchange_weak(tmp, node)) {
              return;
          }
      }
  }

  std::optional<T> Pop() {
      if (head_.load() == nullptr) {
          return std::nullopt;
      } else {
          Node* current_head;
          while (true) {
              current_head = head_.load();
              if (head_.compare_exchange_weak(current_head, current_head->next)) {
                  break;
              }
          }
          Node* node = current_head;
          T res = node->value;
          delete node;
          return res;
      }
  }

 private:
    struct Node {
        T value = T();
        Node* next = nullptr;
    };

  std::atomic<Node*> head_;

};

