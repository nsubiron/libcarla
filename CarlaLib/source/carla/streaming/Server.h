#pragma once

#include "carla/ThreadGroup.h"
#include "carla/streaming/low_level/Server.h"
#include "carla/streaming/low_level/tcp/Server.h"

#include <boost/asio/io_service.hpp>

namespace carla {
namespace streaming {

  class Server {
  public:

    explicit Server(uint16_t port)
      : _server(_io_service, port) {}

    explicit Server(const std::string &address, uint16_t port)
      : _server(_io_service, address, port) {}

    ~Server() {
      Stop();
    }

    Stream MakeStream() {
      return _server.MakeStream();
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

    low_level::Server<low_level::tcp::Server> _server;

    ThreadGroup _workers;
  };

} // namespace streaming
} // namespace carla
