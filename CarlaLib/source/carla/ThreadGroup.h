#pragma once

#include <thread>
#include <vector>

namespace carla {

  class ThreadGroup {
  public:

    ThreadGroup() {}

    ThreadGroup(const ThreadGroup &) = delete;
    ThreadGroup &operator=(const ThreadGroup &) = delete;

    ~ThreadGroup() {
      JoinAll();
    }

    template <typename F>
    void CreateThread(F &&functor) {
      _threads.emplace_back(std::forward<F>(functor));
    }

    template <typename F>
    void CreateThreads(size_t count, F functor) {
      _threads.reserve(_threads.size() + count);
      for (size_t i = 0u; i < count; ++i) {
        CreateThread(functor);
      }
    }

    void JoinAll() {
      for (auto &thread : _threads) {
        if (thread.joinable()) {
          thread.join();
        }
      }
    }

  private:

    std::vector<std::thread> _threads;
  };

} // namespace carla
