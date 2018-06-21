cmake_minimum_required(VERSION 3.9.0)
project(libcarla-client)

# Install rpclib.
install(DIRECTORY "${RPCLIB_INCLUDE_PATH}/rpc" DESTINATION include)
install(FILES "${RPCLIB_LIB_PATH}/librpc.a" DESTINATION lib)

file(GLOB_RECURSE carlalib_client_sources
    "${carlalib_source_path}/carla/networking/client/*.h"
    "${carlalib_source_path}/carla/networking/client/*.cpp")

# Create targets for debug and release in the same build type.
foreach(target carla_client_debug carla_client)
  add_library(${target} STATIC ${carlalib_client_sources})

  target_include_directories(${target} PRIVATE
      "${BOOST_INCLUDE_PATH}"
      "${RPCLIB_INCLUDE_PATH}")

  install(TARGETS ${target} DESTINATION lib)
endforeach(target)

# Specific options for debug.
set_target_properties(carla_client_debug PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
target_compile_definitions(carla_client_debug PUBLIC -DBOOST_ASIO_ENABLE_BUFFER_DEBUGGING)

# Specific options for release.
set_target_properties(carla_client PROPERTIES COMPILE_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})

# Install headers.
install(FILES "${carlalib_source_path}/carla/MsgPack.h"                   DESTINATION include/carla)
install(FILES "${carlalib_source_path}/carla/Version.h"                   DESTINATION include/carla)
install(FILES "${carlalib_source_path}/carla/networking/Networking.h"     DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/Transform.h"      DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/VehicleControl.h" DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/ClientFwdDcl.h"   DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/Client.h"         DESTINATION include/carla/networking)

file(GLOB carlalib_client_headers "${carlalib_source_path}/carla/networking/client/*.h")
install(FILES ${carlalib_client_headers} DESTINATION include/carla/networking/client)
