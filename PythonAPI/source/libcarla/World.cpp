#include <carla/client/Actor.h>
#include <carla/client/World.h>

#include <boost/python.hpp>

void export_world() {
  using namespace boost::python;
  namespace cc = carla::client;

  class_<cc::World, boost::noncopyable, boost::shared_ptr<cc::World>>("World", no_init)
    .def("get_blueprint_library", &cc::World::GetBlueprintLibrary)
    .def("try_spawn_actor", &cc::World::TrySpawnActor)
    .def("spawn_actor", &cc::World::SpawnActor)
  ;
}
