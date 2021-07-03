#pragma once

#include <ostream>
#include <string_view>
#include <thread>
#include <vector>
#include <mutex>


class HelloWorld {
 public:
  static constexpr std::string_view kHelloPrefix = "Hello, World! From thread ";

  HelloWorld(size_t n_threads) : n(n_threads) {
  }

  void SayHello(std::ostream& os) {
      std::mutex m;

      auto printer = [&] {
          std::unique_lock lock(m);
          os << kHelloPrefix << std::this_thread::get_id() << '\n';
      };

      for (size_t i = 0; i < n; ++i) {
          threads.emplace_back(printer);
      }

      for (auto& t : threads)
          t.join();
  }

 private:
  size_t n;
  std::vector<std::thread> threads;
};

