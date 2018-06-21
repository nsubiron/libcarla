#pragma once

#include "carla/MsgPack.h"

namespace carla {
namespace networking {

  class Location {
  public:

    Location() = default;

    Location(float ix, float iy, float iz)
      : x(ix),
        y(iy),
        z(iz) {}

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    MSGPACK_DEFINE_ARRAY(x, y, z);
  };

  class Rotation {
  public:

    Rotation() = default;

    Rotation(float p, float y, float r)
      : pitch(p),
        yaw(y),
        roll(r) {}

    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    MSGPACK_DEFINE_ARRAY(pitch, yaw, roll);
  };

  class Transform {
  public:

    Transform() = default;

    Transform(const Location &in_location, const Rotation &in_rotation)
      : location(in_location),
        rotation(in_rotation) {}

    Location location;
    Rotation rotation;

    MSGPACK_DEFINE_ARRAY(location, rotation);
  };

} // namespace carla
} // namespace networking
