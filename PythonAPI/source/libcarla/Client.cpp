#include <carla/client/Client.h>
#include <carla/client/World.h>

#include <boost/python.hpp>

void export_client() {
  using namespace boost::python;
  namespace cc = carla::client;

  class_<cc::Client, boost::noncopyable, boost::shared_ptr<cc::Client>>("Client", init<std::string, uint16_t>())
    .def("set_timeout", &cc::Client::SetTimeout)
    .def("get_client_version", &cc::Client::GetClientVersion)
    .def("get_server_version", &cc::Client::GetServerVersion)
    .def("ping", &cc::Client::Ping)
    .def("get_world", &cc::Client::GetWorld)
  ;
}
