cmake_minimum_required(VERSION 3.9.0)
project(libcarla)

message(STATUS "Building ${PROJECT_NAME} version ${CARLA_VERSION}")

set(carlalib_source_path "${PROJECT_SOURCE_DIR}/source")

include_directories(${carlalib_source_path})

configure_file(${carlalib_source_path}/carla/Version.h.in ${carlalib_source_path}/carla/Version.h)

add_subdirectory("cmake/tests")
