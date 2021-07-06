#pragma once

#include <vector>
#include <shared_mutex>


template <typename T>
class ThreadSafeVector {
public:
    ThreadSafeVector() = default;

    T operator[](size_t index) const {
        sh_mutex_.lock_shared();
        T result = vector_[index];
        sh_mutex_.unlock_shared();
        return result;
    }

    size_t Size() const {
        return vector_.size();
    }

    void PushBack(const T& value) {
        sh_mutex_.lock();
        vector_.push_back(value);
        sh_mutex_.unlock();
    }

private:
    mutable std::shared_mutex sh_mutex_;
    std::vector<T> vector_;
};


