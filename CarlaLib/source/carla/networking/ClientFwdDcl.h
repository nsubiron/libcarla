#pragma once

#include "carla/networking/Networking.h"

// We use boost::shared_ptr for now to make it compatible with boost::python,
// but it would be nice to make an adaptor for std::shared_ptr.
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace carla {
namespace networking {

namespace client {

  class Client;
  class Vehicle;

} // namespace client

  using Client = ::carla::networking::client::Client;
  using SpClient = boost::shared_ptr<Client>;

  using Vehicle = ::carla::networking::client::Vehicle;
  using SpVehicle = boost::shared_ptr<Vehicle>;

  class Transform;
  class VehicleControl;

} // namespace carla
} // namespace networking
