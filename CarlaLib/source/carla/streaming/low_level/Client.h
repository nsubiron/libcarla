#pragma once

#include "carla/streaming/low_level/Token.h"

#include <boost/asio/io_service.hpp>

namespace carla {
namespace streaming {
namespace low_level {

  /// Wrapper around low level clients.
  template <typename T>
  class Client {

    static auto get_endpoint(const token_type &token) {
      if (!token.protocol_is_tcp())
        throw std::invalid_argument("invalid token, only TCP tokens supported");
      return token.to_tcp_endpoint();
    }

  public:

    using underlying_client = T;

    explicit Client(boost::asio::io_service &io_service, const token_type &token)
      : _client(io_service, get_endpoint(token)),
        _stream_token(token.get_stream_id()) {}

    template <typename F>
    void Subscribe(F &&callback) {
      _client.Subscribe(_stream_token, std::forward<F>(callback));
    }

  private:

    underlying_client _client;

    const stream_id_type _stream_token;
  };

} // namespace low_level
} // namespace streaming
} // namespace carla
