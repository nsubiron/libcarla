#include "test.h"

#include <carla/Version.h>

TEST(misc, version) {
  std::cout << "CarlaLib " << carla::version() << std::endl;
}
