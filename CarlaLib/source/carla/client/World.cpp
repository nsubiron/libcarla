#include "carla/client/World.h"

#include "carla/Logging.h"

namespace carla {
namespace client {

  SharedPtr<Actor> World::TrySpawnActor(
      const ActorBlueprint &blueprint,
      const Transform &transform) {
    try {
      return SpawnActor(blueprint, transform);
    } catch (const std::exception & DEBUG_ONLY(e)) {
      DEBUG_ONLY(log_debug("TrySpawnActor: failed with:", e.what()));
      return nullptr;
    }
  }

} // namespace client
} // namespace carla
