#pragma once

#include <optional>
#include <condition_variable>
#include <mutex>
#include <iostream>


template <typename T>
class UnbufferedChannel{
public:
    UnbufferedChannel() = default;

    void Send(const T& value) {
        std::unique_lock lock(mutex_);

        ready_to_send_ = true;
        message_ = &value;
        cv2_.notify_one();

        while (!closed_ && !ready_to_receive_) {
            cv1_.wait(lock);
        }

        if (closed_) {
            throw std::runtime_error("Channel is closed");
        }

        ready_to_receive_ = false;
    }

    std::optional<T> Recv() {
        std::unique_lock lock(mutex_);

        ready_to_receive_ = true;
        std::optional<T> result = std::nullopt;

        while (!closed_ && !ready_to_send_) {
            cv2_.wait(lock);
        }

        if (closed_) {
            return result;
        }

        result = *message_;
        ready_to_send_ = false;
        cv1_.notify_all();
        return result;
    }

    void Close() {
        closed_ = true;
        cv1_.notify_all();
        cv2_.notify_all();
    }

private:
    bool closed_ = false,
         ready_to_send_ = false,
         ready_to_receive_ = false;
    std::condition_variable cv1_, cv2_;
    std::mutex mutex_;
    T const * message_;
};

