#pragma once

#include "carla/streaming/tcp/ServerSession.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>

#include <memory>
#include <string>

namespace carla {
namespace streaming {
namespace tcp {

  class Server {
  public:

    using endpoint = boost::asio::ip::tcp::endpoint;
    using duration_type = ServerSession::duration_type;

    explicit Server(boost::asio::io_service &io_service, endpoint ep)
      : _acceptor(io_service, std::move(ep)) {}

    template <typename Functor>
    void Listen(duration_type timeout, Functor callback) {
      log_info("starting streaming server at port", _acceptor.local_endpoint().port());
      _acceptor.get_io_service().post([=]() { OpenSession(timeout, callback); });
    }

  private:

    template <typename Functor>
    void OpenSession(duration_type timeout, Functor callback) {
      using boost::system::error_code;

      auto session = std::make_shared<ServerSession>(_acceptor.get_io_service(), timeout);

      auto handle_query = [=](const error_code &ec) {
        if (!ec) {
          session->Open(callback);
        } else {
          log_error("tcp accept error:", ec.message());
        }
      };

      _acceptor.async_accept(session->_socket, [=](error_code ec) {
        // Handle query and open a new session immediately.
        _acceptor.get_io_service().post([=]() { handle_query(ec); });
        OpenSession(timeout, callback);
      });
    }

    boost::asio::ip::tcp::acceptor _acceptor;
  };

} // namespace tcp
} // namespace streaming
} // namespace carla
