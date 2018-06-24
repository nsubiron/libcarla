#pragma once

#include "carla/ThreadGroup.h"
#include "carla/streaming/low_level/Client.h"
#include "carla/streaming/low_level/tcp/Client.h"

#include <boost/asio/io_service.hpp>

namespace carla {
namespace streaming {

  using stream_token = low_level::token_type;

  class Client {
  public:

    explicit Client(const stream_token &token)
      : _client(_io_service, token) {}

    ~Client() {
      Stop();
    }

    template <typename F>
    void Listen(F &&callback) {
      _client.Subscribe(std::forward<F>(callback));
    }

    void Run() {
      _io_service.run();
    }

    void AsyncRun(std::size_t worker_threads) {
      _workers.CreateThreads(worker_threads, [this](){ Run(); });
    }

    void Stop() {
      _io_service.stop();
      _workers.JoinAll();
    }

  private:

    boost::asio::io_service _io_service;

    low_level::Client<low_level::tcp::Client> _client;

    ThreadGroup _workers;
  };

} // namespace streaming
} // namespace carla
