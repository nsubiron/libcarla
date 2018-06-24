#pragma once

#include "carla/Debug.h"
#include "carla/Logging.h"
#include "carla/streaming/Message.h"
#include "carla/streaming/low_level/Types.h"
#include "carla/streaming/low_level/tcp/Timeout.h"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>

#include <array>
#include <memory>

namespace carla {
namespace streaming {
namespace low_level {
namespace tcp {

  /// A TCP server session. When a session opens, it reads from the socket a
  /// stream id object and passes itself to the callback functor. The session
  /// closes itself after @a timeout of inactivity is met.
  class ServerSession
    : public std::enable_shared_from_this<ServerSession>,
      private boost::noncopyable {
  public:

    using socket_type = boost::asio::ip::tcp::socket;
    using duration_type = timeout_type;

    explicit ServerSession(boost::asio::io_service &io_service, duration_type timeout)
      : _socket(io_service),
        _timeout(timeout),
        _deadline(io_service),
        _strand(io_service) {}

    /// Starts the session and calls @a callback after successfully reading the
    /// stream id.
    ///
    /// @pre Callback function signature:
    /// `void(std::shared_ptr<ServerSession>)`.
    template <typename Functor>
    void Open(Functor callback) {
      auto self = shared_from_this(); // To keep myself alive.
      using boost::system::error_code;

      auto handle_query = [this, self, callback](
          const error_code &ec,
          size_t DEBUG_ONLY(bytes_received)) {
        DEBUG_ASSERT_EQ(bytes_received, sizeof(_stream_id));
        if (!ec || ec == boost::asio::error::message_size) {
          log_debug("session{ stream", _stream_id, "} started");
          _socket.get_io_service().post([=]() { callback(self); });
        } else {
          log_error("session{ stream", _stream_id, "} error opening session :", ec.message());
          Close();
        }
      };

      // Read the stream id.
      _deadline.expires_from_now(_timeout);
      _socket.async_receive(
          boost::asio::buffer(&_stream_id, sizeof(_stream_id)),
          _strand.wrap(handle_query));

      StartTimer();
    }

    stream_id_type get_stream_id() const {
      // Note that the stream id isn't synchronized. This function should only be
      // called from the @a callback function, and after that point the stream_id
      // can't change.
      return _stream_id;
    }

    /// Writes some data to the socket.
    void Write(std::shared_ptr<const Message> message) {
      auto self = shared_from_this();
      using boost::system::error_code;

      // Explicitly capturing both objects we ensure they live as long as this
      // lambda. The standard guarantees that explicit captures are not
      // optimized away.
      auto handle_sent = [self, message](const error_code &ec, size_t) {
        if (ec) {
          log_error("session{ stream", self->_stream_id, "} error sending data :", ec.message());
        }
      };

      _deadline.expires_from_now(_timeout);
      _socket.async_send(message->encode(), _strand.wrap(handle_sent));
    }

    void Close() {
      _strand.post([self = shared_from_this(), this]() {
        if (_socket.is_open()) {
          _socket.close();
        }
        log_debug("session { stream", _stream_id, "} closed");
      });
    }

  private:

    void StartTimer() {
      if (_deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now()) {
        log_debug("session { stream", _stream_id, "} timed out");
        Close();
      } else {
        _deadline.async_wait([self = shared_from_this()](boost::system::error_code) {
          self->StartTimer();
        });
      }
    }

    friend class Server;

    stream_id_type _stream_id = 0u;

    socket_type _socket;

    duration_type _timeout;

    boost::asio::deadline_timer _deadline;

    boost::asio::io_service::strand _strand;
  };

} // namespace tcp
} // namespace low_level
} // namespace streaming
} // namespace carla
