#pragma once

#include "carla/Debug.h"
#include "carla/Logging.h"
#include "carla/streaming/Message.h"
#include "carla/streaming/tcp/Encoding.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>

#include <functional>
#include <memory>
#include <string>

namespace carla {
namespace streaming {
namespace tcp {

  class Client {
  public:

    using endpoint = boost::asio::ip::tcp::endpoint;

    explicit Client(boost::asio::io_service &io_service, endpoint ep)
      : _endpoint(std::move(ep)),
        _socket(io_service),
        _strand(io_service) {}

    template <typename Functor>
    void Subscribe(token_type token, Functor callback) {
      log_debug("streaming client: subscribing to stream", token, "at", _endpoint);
      _strand.post([=]() { Connect(token, callback); });
    }

  private:

    template <typename Functor>
    void Connect(token_type token, Functor callback);

    template <typename Functor>
    void ReadData(token_type token, Functor callback);

    endpoint _endpoint;

    boost::asio::ip::tcp::socket _socket;

    boost::asio::io_service::strand _strand;
  };

  template <typename Functor>
  void Client::Connect(token_type token, Functor callback) {
    using boost::system::error_code;

    if (_socket.is_open()) {
      _socket.close();
    }

    auto handle_connect = [=](error_code ec) {
      if (!ec) {
        // Send the token to subscribe to a stream of data.
        log_debug("streaming client: sending token", token);
        _socket.async_send(
            boost::asio::buffer(&token, sizeof(token)),
            _strand.wrap([=](error_code ec, size_t DEBUG_ONLY(bytes)) {
          if (!ec) {
            DEBUG_ASSERT(bytes == sizeof(token));
            // If succeeded start reading data.
            ReadData(token, callback);
          } else {
            // Else try again.
            log_debug("streaming client: failed to send token:", ec.message());
            Connect(token, callback);
          }
        }));
      } else {
        log_debug("streaming client: connection failed:", ec.message());
        Connect(token, callback);
      }
    };

    log_debug("streaming client: connecting to", _endpoint);
    _socket.async_connect(_endpoint, _strand.wrap(handle_connect));
  }

  template <typename Functor>
  void Client::ReadData(token_type token, Functor callback) {
    using boost::system::error_code;

    auto message = std::make_shared<Message>();

    auto handle_read_data = [=](error_code ec, size_t DEBUG_ONLY(bytes)) {
      if (!ec) {
        DEBUG_ASSERT(bytes == message->size());
        // Move the buffer to the callback function and start reading the
        // next
        // piece of data.
        callback(std::move(message));
        ReadData(token, callback);
      } else {
        // As usual, if anything fails start over from the very top.
        log_debug("streaming client: failed to read data:", ec.message());
        Connect(token, callback);
      }
    };

    auto handle_read_header = [=](error_code ec, size_t DEBUG_ONLY(bytes)) {
      if (!ec) {
        DEBUG_ASSERT(bytes == sizeof(message_size_type));
        // Now that we know the size of the coming buffer, we can allocate
        // our buffer and start putting data into it.
        (*message) = Message(message->size());
        _socket.async_receive(
            boost::asio::buffer(message->buffer()),
            _strand.wrap(handle_read_data));
      } else {
        log_debug("streaming client: failed to read header:", ec.message());
        Connect(token, callback);
      }
    };

    // Read the size of the buffer that is coming.
    _socket.async_receive(
        boost::asio::buffer(&(message->_size), sizeof(message->_size)),
        _strand.wrap(handle_read_header));
  }

} // namespace tcp
} // namespace streaming
} // namespace carla
