#include "test.h"

#include <carla/Version.h>

TEST(miscellaneous, version) {
  std::cout << "LibCarla " << carla::version() << std::endl;
}
