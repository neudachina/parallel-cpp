#pragma once

#include <ostream>
#include <string_view>
#include <thread>
#include <vector>
#include <mutex>


class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t n_threads) : n_threads_(n_threads) {
  }

  void SayHello(std::ostream& os) {
      std::mutex mutex;

      auto printer = [&] {
          std::unique_lock lock(mutex);
          os << kHelloPrefix << std::this_thread::get_id() << '\n';
      };

      std::vector<std::thread> threads;
      for (size_t i = 0; i < n_threads_; ++i) {
          threads.emplace_back(printer);
      }

      for (auto& t : threads) {
          t.join();
      }
  }

 private:
  const size_t n_threads_;
};

