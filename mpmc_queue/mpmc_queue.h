#pragma once

#include <optional>
#include <cstdint>
#include <thread>

template <class T>
struct Node {
    T value = T();
    std::atomic<uint64_t> tag = 0;
};

template <class T>
class MPMCQueue {
public:
    explicit MPMCQueue(uint64_t max_size) :
        data_(max_size),
        size_(max_size),
        head_(0),
        tail_(0) {
        for (uint64_t i = 0; i < size_; ++i) {
            data_[i].tag = i;
        }
    }

    bool Push(const T& value) {
        uint64_t idx;
        while (true) {
            uint64_t current_head = head_.load();
            idx = current_head & (size_ - 1);
            if (data_[idx].tag != current_head) {
                if (current_head == tail_.load() + size_) {
                    return false;
                } else {
                    std::this_thread::yield();
                }
            } else {
                if (head_.compare_exchange_weak(current_head, current_head + 1)) {
                    break;
                } else {
                    std::this_thread::yield();
                    continue;
                }
            }
        }
        data_[idx].value = value;
        ++data_[idx].tag;
        return true;
    }

    std::optional<T> Pop() {
        uint64_t idx;
        while (true) {
            uint64_t current_tail = tail_.load();
            idx = current_tail & (size_ - 1);
            if (data_[idx].tag != current_tail + 1) {
                if (current_tail == head_.load()) {
                    return std::nullopt;
                } else {
                    std::this_thread::yield();
                }
            } else {
                if (tail_.compare_exchange_weak(current_tail, current_tail + 1)) {
                    break;
                } else {
                    std::this_thread::yield();
                    continue;
                }
            }
        }

        T value = data_[idx].value;
        data_[idx].tag += size_ - 1;
        return value;
    }

private:
    std::vector<Node<T>> data_;
    uint64_t size_;
    std::atomic<uint64_t> head_, tail_;
};

