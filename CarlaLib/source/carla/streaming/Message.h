#pragma once

#include "carla/Debug.h"

#include <boost/asio/buffer.hpp>

#include <array>
#include <exception>
#include <limits>
#include <memory>
#include <string>

namespace carla {
namespace streaming {

namespace tcp {

  class Client; /// @todo

} // namespace tcp

  /// A message owns a buffer with raw data.
  class Message {

    // =========================================================================
    // -- Typedefs -------------------------------------------------------------
    // =========================================================================

  public:

    using value_type = unsigned char;

    using size_type = uint32_t;

    // =========================================================================
    // -- Construction and assignment ------------------------------------------
    // =========================================================================

  public:

    Message() = default;

    explicit Message(size_type size)
      : _size(size),
        _data(std::make_unique<value_type[]>(_size)) {}

    explicit Message(uint64_t size)
      : Message([size]() {
          if (size > std::numeric_limits<size_type>::max()) {
            throw std::invalid_argument("message size too big");
          }
          return static_cast<size_type>(size);
        } ()) {}

    template <typename ConstBufferSequence>
    explicit Message(ConstBufferSequence source)
      : Message(boost::asio::buffer_size(source)) {
      DEBUG_ONLY(auto bytes_copied = )
      boost::asio::buffer_copy(buffer(), source);
      DEBUG_ASSERT(bytes_copied == size());
    }

    Message(const Message &) = delete;
    Message &operator=(const Message &) = delete;

    Message(Message &&rhs) noexcept
      : _size(rhs._size),
        _data(std::move(rhs._data)) {
      rhs._size = 0u;
    }

    Message &operator=(Message &&rhs) noexcept {
      _size = rhs._size;
      _data = std::move(rhs._data);
      rhs._size = 0u;
      return *this;
    }

    // =========================================================================
    // -- Data access ----------------------------------------------------------
    // =========================================================================

  public:

    size_type size() const {
      return _size;
    }

    const value_type *data() const {
      return _data.get();
    }

    value_type *data() {
      return _data.get();
    }

    // =========================================================================
    // -- Conversions ----------------------------------------------------------
    // =========================================================================

  public:

    boost::asio::const_buffer buffer() const {
      return {data(), size()};
    }

    boost::asio::mutable_buffer buffer() {
      return {data(), size()};
    }

    std::array<boost::asio::const_buffer, 2u> encode() const {
      return {boost::asio::buffer(&_size, sizeof(_size)), buffer()};
    }

    std::string reinterpret_as_string() const {
      return {reinterpret_cast<const char *>(data()), size()};
    }

    // =========================================================================
    // -- Private members ------------------------------------------------------
    // =========================================================================

  private:

    friend class tcp::Client; /// @todo

    size_type _size = 0u;

    std::unique_ptr<value_type[]> _data = nullptr;
  };

} // namespace streaming
} // namespace carla