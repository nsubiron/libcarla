cmake_minimum_required(VERSION 3.9.0)
project(libcarla-unit-tests)

file(GLOB_RECURSE carlalib_test_sources
    "${carlalib_source_path}/test/*.h"
    "${carlalib_source_path}/test/*.cpp")

set(carlib_test_link_libraries "-lgtest_main -lgtest")

add_executable(carlalib_test_debug ${carlalib_test_sources})
set_target_properties(carlalib_test_debug PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
target_compile_definitions(carlalib_test_debug PUBLIC -DBOOST_ASIO_ENABLE_BUFFER_DEBUGGING)
target_link_libraries(carlalib_test_debug ${carlib_test_link_libraries})
install(TARGETS carlalib_test_debug DESTINATION test)

add_executable(carlalib_test_release ${carlalib_test_sources})
set_target_properties(carlalib_test_release PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
target_link_libraries(carlalib_test_release ${carlib_test_link_libraries})
install(TARGETS carlalib_test_release DESTINATION test)
