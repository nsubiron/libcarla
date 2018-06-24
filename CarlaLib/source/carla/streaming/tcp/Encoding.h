#pragma once

#include <cstdint>

namespace carla {
namespace streaming {
namespace tcp {

  using token_type = uint32_t;

  using message_size_type = uint32_t; /// @todo use Message::size_type

} // namespace tcp
} // namespace streaming
} // namespace carla
