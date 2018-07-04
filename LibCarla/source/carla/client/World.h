// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/Debug.h"
#include "carla/NonCopyable.h"
#include "carla/client/Client.h"
#include "carla/client/Memory.h"

namespace carla {
namespace client {

  class Actor;

  class World
    : public EnableSharedFromThis<World>,
      private NonCopyable {
  public:

    World(World &&) = default;
    World &operator=(World &&) = default;

    BlueprintLibrary GetBlueprintLibrary() const {
      return _parent->GetBlueprintLibrary();
    }

    SharedPtr<Actor> TrySpawnActor(
        const ActorBlueprint &blueprint,
        const Transform &transform);

    SharedPtr<Actor> SpawnActor(
        const ActorBlueprint &blueprint,
        const Transform &transform) {
      return _parent->SpawnActor(blueprint, transform);
    }

    template <typename ControlType>
    void ApplyControlToActor(const Actor &actor, const ControlType &control) {
      _parent->ApplyControlToActor(actor, control);
    }

  private:

    friend class Client;

    explicit World(SharedPtr<Client> parent) : _parent(std::move(parent)) {
      DEBUG_ASSERT(parent != nullptr);
    }

    SharedPtr<Client> _parent;
  };

} // namespace client
} // namespace carla
