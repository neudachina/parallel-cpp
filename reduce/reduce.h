#pragma once

#include <cstddef>
#include <vector>
#include <thread>

template <typename RandomAccessIterator, class T, class Func>
void reducer(RandomAccessIterator begin, RandomAccessIterator end, Func func, T* value) {
    T result = *begin;
    for (RandomAccessIterator it = begin + 1; it != end; ++it) {
        result = func(result, *it);
    }
    *value = result;
}

template <typename RandomAccessIterator, class T, class Func>
T parallel_reduce(RandomAccessIterator begin, RandomAccessIterator end,
                  const T& initial_value, Func func, size_t n_threads) {
    std::vector<T> threads_res(n_threads);
    std::vector<std::thread> threads;
    size_t size = (end - begin) / n_threads + ((end - begin) % n_threads != 0);

    for (size_t i = 0; i < n_threads; ++i) {
        threads.emplace_back(reducer<RandomAccessIterator, T, Func>,
                             begin + size * i, std::min(begin + size * (i + 1), end), func, &threads_res[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    threads_res.push_back(initial_value);

    T result;
    reducer(threads_res.begin(), threads_res.end(), func, &result);

    return result;
}

