#pragma once

#include "carla/networking/ClientFwdDcl.h"

namespace carla {
namespace networking {
namespace client {

  class Vehicle : public boost::enable_shared_from_this<Vehicle> {
  public:

    using id_type = agent_id_type;

    ~Vehicle();

    id_type GetId() const {
      return _id;
    }

    bool ApplyControl(const VehicleControl &control);

  private:

    friend class Client;

    Vehicle(id_type id, SpClient parent);

    id_type _id;

    SpClient _parent;
  };

} // namespace carla
} // namespace networking
} // namespace client
