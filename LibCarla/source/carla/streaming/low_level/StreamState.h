// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/streaming/Message.h"
#include "carla/streaming/low_level/Session.h"
#include "carla/streaming/low_level/Token.h"

#include <memory>
#include <mutex>

namespace carla {
namespace streaming {
namespace low_level {

namespace detail {

  /// Handles the synchronization of the shared session.
  class SessionHolder {
  public:

    void set_session(std::shared_ptr<Session> session) {
      std::lock_guard<std::mutex> guard(_mutex);
      _session = std::move(session);
    }

  protected:

    std::shared_ptr<Session> get_session() const {
      std::lock_guard<std::mutex> guard(_mutex);
      return _session;
    }

  private:

    mutable std::mutex _mutex; /// @todo it can be atomic

    std::shared_ptr<Session> _session;
  };

} // namespace detail

  /// Shared state among all the copies of a stream. Provides access to the
  /// underlying UDP session if active.
  class StreamState
    : public detail::SessionHolder,
      private boost::noncopyable {
  public:

    explicit StreamState(const token_type &token) : _token(token) {}

    const token_type &token() const {
      return _token;
    }

    void Write(std::shared_ptr<const Message> message) {
      auto session = get_session();
      if (session != nullptr) {
        session->Write(message);
      }
    }

  private:

    const token_type _token;
  };

} // namespace low_level
} // namespace streaming
} // namespace carla
