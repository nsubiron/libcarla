// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "test.h"

#include <carla/ThreadGroup.h>
#include <carla/rpc/Client.h>
#include <carla/rpc/Server.h>

#include <thread>

using namespace carla::rpc;

TEST(rpc, compilation_tests) {
  Server server(TESTING_PORT);
  server.BindSync("bind00", []() { return 2.0f; });
  server.BindSync("bind01", [](int x) { return x; });
  server.BindSync("bind02", [](int, float) { return 0.0; });
  server.BindSync("bind03", [](int, float, double, char) {});
}

TEST(rpc, server_bind_sync_run_on_game_thread) {
  const auto main_thread_id = std::this_thread::get_id();

  Server server(TESTING_PORT);

  server.BindSync("do_the_thing", [=](int x, int y) -> int {
    EXPECT_EQ(std::this_thread::get_id(), main_thread_id);
    return x + y;
  });

  server.AsyncRun(1u);

  std::atomic_bool done{false};

  carla::ThreadGroup threads;
  threads.CreateThread([&]() {
    Client client("localhost", TESTING_PORT);
    for (auto i = 0u; i < 300u; ++i) {
      auto result = client.call("do_the_thing", i, 1).as<int>();
      EXPECT_EQ(result, i + 1);
    }
    done = true;
  });

  auto i = 0u;
  for (; i < 1'000'000u; ++i) {
    server.SyncRunFor(2ms);
    if (done) {
      break;
    }
  }
  std::cout << "game thread: run " << i << " slices.\n";
  ASSERT_TRUE(done);
}
