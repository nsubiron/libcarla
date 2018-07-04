// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/Debug.h"
#include "carla/NonCopyable.h"
#include "carla/client/Control.h"
#include "carla/client/Memory.h"
#include "carla/client/World.h"
#include "carla/rpc/Actor.h"

namespace carla {
namespace client {

  class Client;

  class Actor
    : public EnableSharedFromThis<Actor>,
      private NonCopyable {
  public:

    Actor(Actor &&) = default;
    Actor &operator=(Actor &&) = default;

    auto GetId() const {
      return _actor.id;
    }

    const std::string &GetTypeId() const {
      return _actor.blueprint.type_id;
    }

    ActorBlueprint GetBlueprint() const {
      return _actor.blueprint;
    }

    SharedPtr<World> GetWorld() const {
      return _world;
    }

    void ApplyControl(const VehicleControl &control) {
      _world->ApplyControlToActor(*this, control);
    }

    const auto &Serialize() const {
      return _actor;
    }

  private:

    friend class Client;

    Actor(carla::rpc::Actor actor, SharedPtr<World> world)
      : _actor(actor),
        _world(std::move(world)) {
      DEBUG_ASSERT(_world != nullptr);
    }

    carla::rpc::Actor _actor;

    SharedPtr<World> _world;
  };

} // namespace client
} // namespace carla
