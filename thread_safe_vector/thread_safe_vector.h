#pragma once

#include <vector>
#include <shared_mutex>


template <typename T>
class ThreadSafeVector {
public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        std::shared_lock lock(read_mutex_);
        T result = vector_[index];
        return result;
    }

    size_t Size() const {
        return size_;
    }

    void PushBack(const T& value) {
        std::unique_lock w_lock(write_mutex_);
        if (vector_.size() == vector_.capacity()) {
            std::unique_lock r_lock(read_mutex_);
            vector_.push_back(value);
            ++size_;
        } else {
            vector_.push_back(value);
            ++size_;
        }
    }

private:
    mutable std::shared_mutex read_mutex_;
    mutable std::mutex write_mutex_;
    std::vector<T> vector_;
    std::atomic<size_t> size_;
};


