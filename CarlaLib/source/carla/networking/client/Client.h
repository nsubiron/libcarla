#pragma once

#include "carla/Version.h"
#include "carla/networking/ClientFwdDcl.h"
#include "carla/networking/Transform.h"
#include "carla/networking/VehicleControl.h"
#include "carla/networking/client/Vehicle.h"

#include <rpc/client.h>

#include <string>

namespace carla {
namespace networking {
namespace client {

  class Client : public boost::enable_shared_from_this<Client> {
  public:

    template <typename ... Args>
    explicit Client(Args && ... args)
      : _client(std::forward<Args>(args) ...) {}

    template <typename T, typename ... Args>
    T Call(const std::string &function, Args && ... args) {
      return _client.call(function, std::forward<Args>(args) ...).template as<T>();
    }

    std::string GetClientVersion() const {
      return ::carla::version();
    }

    std::string GetServerVersion() {
      return Call<std::string>("version");
    }

    bool Ping() {
      return Call<bool>("ping");
    }

    SpVehicle SpawnVehicle(Transform transform) {
      const auto id = Call<agent_id_type>("spawn", transform);
      if (id < 0) {
        return nullptr;
      }
      return SpVehicle{new Vehicle(id, shared_from_this())};
    }

    bool ApplyControl(Vehicle &vehicle, const VehicleControl &control) {
      return Call<bool>("control", vehicle.GetId(), control);
    }

  private:

    rpc::client _client;
  };

} // namespace carla
} // namespace networking
} // namespace client
