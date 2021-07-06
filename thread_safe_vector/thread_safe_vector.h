#pragma once

#include <vector>
#include <shared_mutex>


template <typename T>
class ThreadSafeVector {
public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        read_mutex_.lock_shared();
        T result = vector_[index];
        read_mutex_.unlock_shared();
        return result;
    }

    size_t Size() const {
        return vector_.size();
    }

    void PushBack(const T& value) {
        write_mutex_.lock();
        if (vector_.size() == vector_.capacity()) {
            read_mutex_.lock();
            vector_.push_back(value);
            read_mutex_.unlock();
        } else {
            vector_.push_back(value);
        }
        write_mutex_.unlock();
    }

private:
    mutable std::shared_mutex read_mutex_;
    mutable std::mutex write_mutex_;
    std::vector<T> vector_;
};


