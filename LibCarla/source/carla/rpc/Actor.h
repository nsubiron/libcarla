#pragma once

#include "carla/rpc/ActorBlueprint.h"

namespace carla {
namespace rpc {

  class Actor {
  public:

    using id_type = uint32_t;

    id_type id;

    ActorBlueprint blueprint;

    MSGPACK_DEFINE_ARRAY(id, blueprint);
  };

} // namespace rpc
} // namespace carla
