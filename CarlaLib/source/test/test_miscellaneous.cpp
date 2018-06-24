#include "test.h"

#include <carla/Version.h>

TEST(miscellaneous, version) {
  std::cout << "CarlaLib " << carla::version() << std::endl;
}
