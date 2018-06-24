#pragma once

#include "carla/Debug.h"
#include "carla/streaming/Message.h"
#include "carla/streaming/low_level/StreamState.h"
#include "carla/streaming/low_level/Token.h"

#include <boost/asio/buffer.hpp>

#include <memory>

namespace carla {
namespace streaming {

namespace low_level {

  class Dispatcher;

} // namespace low_level

  using stream_token = low_level::token_type;

  class Stream {
  public:

    Stream() = delete;

    Stream(const Stream &) = default;
    Stream(Stream &&) = default;

    Stream &operator=(const Stream &) = default;
    Stream &operator=(Stream &&) = default;

    stream_token token() const {
      return _shared_state->token();
    }

    template <typename ConstBufferSequence>
    void Write(ConstBufferSequence buffer) {
      _shared_state->Write(std::make_shared<Message>(buffer));
    }

    template <typename T>
    Stream &operator<<(const T &rhs) {
      Write(boost::asio::buffer(rhs));
      return *this;
    }

  private:

    friend class low_level::Dispatcher;

    Stream(std::shared_ptr<low_level::StreamState> state)
      : _shared_state(std::move(state)) {
      DEBUG_ASSERT(_shared_state != nullptr);
    }

    std::shared_ptr<low_level::StreamState> _shared_state;
  };

} // namespace streaming
} // namespace carla
