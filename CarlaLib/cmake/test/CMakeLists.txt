cmake_minimum_required(VERSION 3.9.0)
project(libcarla-unit-tests)

file(GLOB_RECURSE carlalib_test_sources
    "${carlalib_source_path}/carla/profiler/*.h"
    "${carlalib_source_path}/carla/profiler/*.cpp"
    "${carlalib_source_path}/test/*.h"
    "${carlalib_source_path}/test/*.cpp")

link_directories(
    ${RPCLIB_LIB_PATH}
    ${GTEST_LIB_PATH})

# Create targets for debug and release in the same build type.
foreach(target carlalib_test_debug carlalib_test_release)
  add_executable(${target} ${carlalib_test_sources})

  target_compile_definitions(carlalib_test_debug PUBLIC
      -DCARLALIB_ENABLE_PROFILER
      -DCARLALIB_WITH_GTEST)

  target_include_directories(${target} PRIVATE
      "${BOOST_INCLUDE_PATH}"
      "${RPCLIB_INCLUDE_PATH}"
      "${GTEST_INCLUDE_PATH}")
  target_link_libraries(${target} "-lrpc -lgtest_main -lgtest")

  install(TARGETS ${target} DESTINATION test)
endforeach(target)

# Specific options for debug.
set_target_properties(carlalib_test_debug PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
target_compile_definitions(carlalib_test_debug PUBLIC -DBOOST_ASIO_ENABLE_BUFFER_DEBUGGING)

# Specific options for release.
set_target_properties(carlalib_test_release PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
