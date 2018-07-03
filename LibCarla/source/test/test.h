#pragma once

#ifndef NDEBUG
#  define LIBCARLA_LOG_LEVEL LIBCARLA_LOG_LEVEL_INFO
#endif // NDEBUG

#include "test/util/Message.h"

#include <carla/Logging.h>
#include <carla/profiler/Profiler.h>

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <iostream>

constexpr uint16_t TESTING_PORT = 8080u;

using namespace std::chrono_literals;
