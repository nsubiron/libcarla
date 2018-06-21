cmake_minimum_required(VERSION 3.9.0)
project(libcarla-server)

# Install libc++ shared libraries.
file(GLOB LibCXX_Libraries "${LLVM_LIB_PATH}/libc++*")
install(FILES ${LibCXX_Libraries} DESTINATION lib)

# Install rpclib.
install(DIRECTORY "${RPCLIB_INCLUDE_PATH}/rpc" DESTINATION include)
install(FILES "${RPCLIB_LIB_PATH}/librpc.a" DESTINATION lib)

# Install libcarla-server.
install(FILES "${carlalib_source_path}/carla/MsgPack.h"                   DESTINATION include/carla)
install(FILES "${carlalib_source_path}/carla/Version.h"                   DESTINATION include/carla)
install(FILES "${carlalib_source_path}/carla/networking/Networking.h"     DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/Transform.h"      DESTINATION include/carla/networking)
install(FILES "${carlalib_source_path}/carla/networking/VehicleControl.h" DESTINATION include/carla/networking)
