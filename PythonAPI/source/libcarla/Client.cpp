#include <carla/networking/Client.h>

#include <boost/python.hpp>

#include <ostream>

namespace carla {
namespace networking {

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

  std::ostream &operator<<(std::ostream &out, const VehicleControl &control) {
    auto boolalpha = [](bool b) { return b ? "True" : "False"; };
    out << "VehicleControl(throttle=" << control.throttle
        << ", steer=" << control.steer
        << ", brake=" << control.brake
        << ", hand_brake=" << boolalpha(control.hand_brake)
        << ", reverse=" << boolalpha(control.reverse) << ')';
    return out;
  }

namespace client {

  std::ostream &operator<<(std::ostream &out, const Vehicle &vehicle) {
    out << "Vehicle(id=" << vehicle.GetId() << ')';
    return out;
  }

} // namespace client

} // namespace networking
} // namespace carla

BOOST_PYTHON_MODULE(libcarla) {
  using namespace boost::python;
  namespace cn = carla::networking;

  class_<cn::Location>("Location")
    .def(init<float, float, float>((arg("x")=0.0f, arg("y")=0.0f, arg("z")=0.0f)))
    .def_readwrite("x", &cn::Location::x)
    .def_readwrite("y", &cn::Location::y)
    .def_readwrite("z", &cn::Location::z)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cn::Rotation>("Rotation")
    .def(init<float, float, float>((arg("pitch")=0.0f, arg("yaw")=0.0f, arg("roll")=0.0f)))
    .def_readwrite("pitch", &cn::Rotation::pitch)
    .def_readwrite("yaw", &cn::Rotation::yaw)
    .def_readwrite("roll", &cn::Rotation::roll)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cn::Transform>("Transform")
    .def(init<cn::Location, cn::Rotation>(
        (arg("location")=cn::Location(), arg("rotation")=cn::Rotation())))
    .def_readwrite("location", &cn::Transform::location)
    .def_readwrite("rotation", &cn::Transform::rotation)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cn::VehicleControl>("VehicleControl")
    .def(init<float, float, float, bool, bool>(
        (arg("throttle")=0.0f,
         arg("steer")=0.0f,
         arg("brake")=0.0f,
         arg("hand_brake")=false,
         arg("reverse")=false)))
    .def_readwrite("throttle", &cn::VehicleControl::throttle)
    .def_readwrite("steer", &cn::VehicleControl::steer)
    .def_readwrite("brake", &cn::VehicleControl::brake)
    .def_readwrite("hand_brake", &cn::VehicleControl::hand_brake)
    .def_readwrite("reverse", &cn::VehicleControl::reverse)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cn::Vehicle, boost::noncopyable, boost::shared_ptr<cn::Vehicle>>("Vehicle", no_init)
    .add_property("id", &cn::Vehicle::GetId)
    .def("apply_control", &cn::Vehicle::ApplyControl)
    .def(self_ns::str(self_ns::self))
  ;

  class_<cn::Client, boost::noncopyable, boost::shared_ptr<cn::Client>>("Client", init<std::string, uint16_t>())
    .def("get_client_version", &cn::Client::GetClientVersion)
    .def("get_server_version", &cn::Client::GetServerVersion)
    .def("ping", &cn::Client::Ping)
    .def("spawn_vehicle", &cn::Client::SpawnVehicle)
  ;
}
