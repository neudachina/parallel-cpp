#pragma once

#include <atomic>
#include <thread>

struct Node {
    std::atomic<bool> blocked = false;
    std::atomic<Node*> next = nullptr;
};

class MCSLock {
public:
    MCSLock() : tail_(nullptr) {}

    void Lock() {
        Node* prev = tail_.exchange(&node_);
        if (prev) {
            node_.blocked = true;
            prev->next = &node_;
            while (node_.blocked) {
                std::this_thread::yield();
            }
        }

    }

    void Unlock() {
        Node* expected = &node_;
        if (!node_.next) {
            if (tail_.compare_exchange_weak(expected, nullptr)) {
                return;
            }
        }

        while (!node_.next) {
            std::this_thread::yield();
        }
        static_cast<Node*>(node_.next)->blocked = false;
    }

private:
    thread_local static Node node_;
    std::atomic<Node*> tail_;
};
