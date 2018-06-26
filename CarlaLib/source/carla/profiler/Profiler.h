#pragma once

#ifndef CARLALIB_ENABLE_PROFILER
#  define CARLALIB_PROFILE_SCOPE(context, profiler_name)
#  define CARLALIB_PROFILE_FPS(context, profiler_name)
#else

#include "carla/StopWatch.h"

#include <algorithm>
#include <string>

namespace carla {
namespace profiler {
namespace detail {

  class ProfilerData {
  public:

    explicit ProfilerData(std::string name, bool print_fps = false)
      : _name(std::move(name)),
        _print_fps(print_fps) {}

    ~ProfilerData();

    void Annotate(const StopWatch &stop_watch) {
      const size_t elapsed_microseconds =
          stop_watch.GetElapsedTime<std::chrono::microseconds>();
      ++_count;
      _total_microseconds += elapsed_microseconds;
      _max_elapsed = std::max(elapsed_microseconds, _max_elapsed);
      _min_elapsed = std::min(elapsed_microseconds, _min_elapsed);
    }

    float average() const {
      return ms(_total_microseconds) / static_cast<float>(_count);
    }

    float maximum() const {
      return ms(_max_elapsed);
    }

    float minimum() const {
      return ms(_min_elapsed);
    }

  private:

    static inline float ms(size_t microseconds) {
      return 1e-3f * static_cast<float>(microseconds);
    }

    static inline float fps(float milliseconds) {
      return 1e3f / milliseconds;
    }

    const std::string _name;

    const bool _print_fps;

    size_t _count = 0u;

    size_t _total_microseconds = 0u;

    size_t _max_elapsed = 0u;

    size_t _min_elapsed = -1;
  };

  class ScopedProfiler {
  public:

    explicit ScopedProfiler(ProfilerData &parent) : _profiler(parent) {}

    ~ScopedProfiler() {
      _stop_watch.Stop();
      _profiler.Annotate(_stop_watch);
    }

  private:

    ProfilerData &_profiler;

    StopWatch _stop_watch;
  };

} // namespace detail
} // namespace profiler
} // namespace carla

#ifdef CARLALIB_WITH_GTEST
#  define CARLALIB_GTEST_GET_TEST_NAME() std::string(::testing::UnitTest::GetInstance()->current_test_info()->name())
#else
#  define CARLALIB_GTEST_GET_TEST_NAME() std::string("")
#endif // CARLALIB_WITH_GTEST

#define CARLALIB_PROFILE_SCOPE(context, profiler_name) \
    static thread_local ::carla::profiler::detail::ProfilerData carla_profiler_ ## context ## _ ## profiler_name ## _data( \
        CARLALIB_GTEST_GET_TEST_NAME() + "." #context "." #profiler_name); \
    ::carla::profiler::detail::ScopedProfiler carla_profiler_ ## context ## _ ## profiler_name ## _scoped_profiler( \
        carla_profiler_ ## context ## _ ## profiler_name ## _data);

#define CARLALIB_PROFILE_FPS(context, profiler_name) \
    { \
      static thread_local ::carla::StopWatch stop_watch; \
      stop_watch.Stop(); \
      static thread_local bool first_time = true; \
      if (!first_time) { \
        static thread_local ::carla::profiler::detail::ProfilerData profiler_data( \
            CARLALIB_GTEST_GET_TEST_NAME() + "." #context "." #profiler_name, true); \
        profiler_data.Annotate(stop_watch); \
      } else { \
        first_time = false; \
      } \
      stop_watch.Restart(); \
    }

#endif // CARLALIB_ENABLE_PROFILER