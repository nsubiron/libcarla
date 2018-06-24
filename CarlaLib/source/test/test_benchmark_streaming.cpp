#include "test.h"

#include <carla/streaming/Client.h>
#include <carla/streaming/Server.h>

using namespace carla::streaming;

static auto non_endl(size_t size) {
  std::vector<char> v(size, 'a');
  return std::make_shared<Message>(boost::asio::buffer(v));
}

static void benchmark_image(
    const size_t dimensions,
    const size_t server_threads = 1u,
    const size_t client_threads = 1u) {
  constexpr auto number_of_messages = 30u;

  Server server(TESTING_PORT);
  Stream stream = server.MakeStream();
  server.AsyncRun(server_threads);

  Client client(stream.token());
  client.Listen([](std::shared_ptr<Message>){
    CARLALIB_PROFILE_FPS(client, listen_callback);
  });
  client.AsyncRun(client_threads);
  std::this_thread::sleep_for(2s);

  carla::ThreadGroup threads;
  threads.CreateThreads(server_threads, [=]() mutable {
    const auto msg = non_endl(dimensions * 4u);
    ASSERT_EQ(msg->size(), dimensions * 4u);
    for (auto i = 0u; i < number_of_messages; ++i) {
      CARLALIB_PROFILE_SCOPE(game, write_to_stream);
      stream << msg->buffer();
    }
  });

  std::this_thread::sleep_for(5s);

  threads.JoinAll();
  server.Stop();
  client.Stop();

  std::this_thread::sleep_for(5s);
}

TEST(benchmark_streaming, image_2x2) {
  benchmark_image(2u * 2u);
}

TEST(benchmark_streaming, image_200x200) {
  benchmark_image(200u * 200u);
}

TEST(benchmark_streaming, image_800x600) {
  benchmark_image(800u * 600u);
}

TEST(benchmark_streaming, image_1920x1080) {
  benchmark_image(1920u * 1080u);
}
