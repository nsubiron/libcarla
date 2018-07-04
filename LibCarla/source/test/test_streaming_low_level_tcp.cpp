// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "test.h"

#include <carla/ThreadGroup.h>
#include <carla/streaming/low_level/tcp/Client.h>
#include <carla/streaming/low_level/tcp/Server.h>

#include <atomic>

TEST(streaming_low_level_tcp, small_message) {
  using namespace util::message;
  using namespace carla::streaming::low_level;
  using shared_session = std::shared_ptr<tcp::ServerSession>;

  boost::asio::io_service io_service;
  tcp::Server::endpoint ep(boost::asio::ip::tcp::v4(), TESTING_PORT);

  tcp::Server srv(io_service, ep);
  srv.set_timeout(1s);
  std::atomic_bool done{false};
  std::atomic_size_t message_count{0u};

  srv.Listen([&](std::shared_ptr<tcp::ServerSession> session) {
    ASSERT_EQ(session->get_stream_id(), 42u);
    const std::string msg = "Hola!";
    auto message = std::make_shared<Message>(boost::asio::buffer(msg));
    while (!done) {
      session->Write(message);
      std::this_thread::sleep_for(1ns);
    }
    std::cout << "done!\n";
  });

  tcp::Client c(io_service, ep, 42u, [&](std::shared_ptr<Message> message) {
    ++message_count;
    ASSERT_NE(message, nullptr);
    ASSERT_EQ(message->size(), 5u);
    const std::string msg = as_string(*message);
    ASSERT_EQ(msg, std::string("Hola!"));
  });

  // We need at least two threads because this server loop consumes one.
  carla::ThreadGroup threads;
  threads.CreateThreads(
      std::max(2u, std::thread::hardware_concurrency()),
      [&]() { io_service.run(); });

  std::this_thread::sleep_for(2s);
  io_service.stop();
  done = true;
  std::cout << "client received " << message_count << " messages\n";
}
