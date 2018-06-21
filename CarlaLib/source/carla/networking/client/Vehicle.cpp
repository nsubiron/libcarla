#include "carla/Debug.h"
#include "carla/networking/client/Client.h"
#include "carla/networking/client/Vehicle.h"

namespace carla {
namespace networking {
namespace client {

  Vehicle::~Vehicle() {}

  bool Vehicle::ApplyControl(const VehicleControl &control) {
    return _parent->ApplyControl(*this, control);
  }

  Vehicle::Vehicle(id_type id, SpClient parent)
    : _id(id),
      _parent(parent) {}

} // namespace carla
} // namespace networking
} // namespace client
