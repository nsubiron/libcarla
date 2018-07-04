// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <carla/client/Transform.h>

#include <boost/python.hpp>

#include <ostream>

namespace carla {
namespace rpc {

  std::ostream &operator<<(std::ostream &out, const Location &location) {
    out << "Location(x=" << location.x
        << ", y=" << location.y
        << ", z=" << location.z << ')';
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const Rotation &rotation) {
    out << "Rotation(pitch=" << rotation.pitch
        << ", yaw=" << rotation.yaw
        << ", roll=" << rotation.roll << ')';
    return out;
  }

  std::ostream &operator<<(std::ostream &out, const Transform &transform) {
    out << "Transform(" << transform.location << ", " << transform.rotation << ')';
    return out;
  }

} // namespace rpc
} // namespace carla

void export_transform() {
  using namespace boost::python;
  namespace cc = carla::client;

  class_<cc::Location>("Location")
    .def(init<float, float, float>((arg("x")=0.0f, arg("y")=0.0f, arg("z")=0.0f)))
    .def_readwrite("x", &cc::Location::x)
    .def_readwrite("y", &cc::Location::y)
    .def_readwrite("z", &cc::Location::z)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::Rotation>("Rotation")
    .def(init<float, float, float>((arg("pitch")=0.0f, arg("yaw")=0.0f, arg("roll")=0.0f)))
    .def_readwrite("pitch", &cc::Rotation::pitch)
    .def_readwrite("yaw", &cc::Rotation::yaw)
    .def_readwrite("roll", &cc::Rotation::roll)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cc::Transform>("Transform")
    .def(init<cc::Location, cc::Rotation>(
        (arg("location")=cc::Location(), arg("rotation")=cc::Rotation())))
    .def_readwrite("location", &cc::Transform::location)
    .def_readwrite("rotation", &cc::Transform::rotation)
    .def(self_ns::str(self_ns::self))
  ;
}
