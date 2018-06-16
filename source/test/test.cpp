#include <carla/Version.h>

#include <gtest/gtest.h>

#include <iostream>

TEST(misc, version) {
  std::cout << "CarlaLib " << carla::version() << std::endl;
}
