#! /bin/bash

# ==============================================================================
# -- Set up environment --------------------------------------------------------
# ==============================================================================

command -v /usr/bin/clang++-5.0 >/dev/null 2>&1 || {
  echo >&2 "clang 5.0 is required, but it's not installed.";
  echo >&2 "make sure you build Unreal Engine with clang 5.0 too.";
  exit 1;
}

export CC=/usr/bin/clang-5.0
export CXX=/usr/bin/clang++-5.0

source $(dirname "$0")/Environment.sh

mkdir -p ${CARLA_BUILD_FOLDER}
pushd ${CARLA_BUILD_FOLDER} >/dev/null

# ==============================================================================
# -- Get and compile libc++ ----------------------------------------------------
# ==============================================================================

LLVM_BASENAME=llvm-5.0

LLVM_INCLUDE=${PWD}/${LLVM_BASENAME}-install/include/c++/v1
LLVM_LIBPATH=${PWD}/${LLVM_BASENAME}-install/lib

if [[ -d "${LLVM_BASENAME}-install" ]] ; then
  log "${LLVM_BASENAME} already installed."
else
  rm -Rf ${LLVM_BASENAME}-source ${LLVM_BASENAME}-build

  log "Retrieving libc++..."

  git clone --depth=1 -b release_50  https://github.com/llvm-mirror/llvm.git ${LLVM_BASENAME}-source
  git clone --depth=1 -b release_50  https://github.com/llvm-mirror/libcxx.git ${LLVM_BASENAME}-source/projects/libcxx
  git clone --depth=1 -b release_50  https://github.com/llvm-mirror/libcxxabi.git ${LLVM_BASENAME}-source/projects/libcxxabi

  log "Compiling libc++..."

  mkdir -p ${LLVM_BASENAME}-build

  pushd ${LLVM_BASENAME}-build >/dev/null

  cmake -G "Ninja" \
      -DLIBCXX_ENABLE_EXPERIMENTAL_LIBRARY=OFF -DLIBCXX_INSTALL_EXPERIMENTAL_LIBRARY=OFF \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX="../${LLVM_BASENAME}-install" \
      ../${LLVM_BASENAME}-source

  ninja cxx

  ninja install-libcxx

  ninja install-libcxxabi

  popd >/dev/null

  # Workaround, it seems LLVM 5.0 does not install these files.
  cp -v ${LLVM_BASENAME}-build/include/c++/v1/cxxabi.h ${LLVM_INCLUDE}
  cp -v ${LLVM_BASENAME}-build/include/c++/v1/__cxxabi_config.h ${LLVM_INCLUDE}

  rm -Rf ${LLVM_BASENAME}-source ${LLVM_BASENAME}-build

fi

# ==============================================================================
# -- Get boost includes --------------------------------------------------------
# ==============================================================================

BOOST_BASENAME=boost-1.67.0

BOOST_INCLUDE=${PWD}/${BOOST_BASENAME}-install/

if [[ -d "${BOOST_BASENAME}-install" ]] ; then
  log "${BOOST_BASENAME} already installed."
else

  log "Retrieving boost..."
  wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
  log "Extracting boost..."
  tar -xzf boost_1_67_0.tar.gz
  rm boost_1_67_0.tar.gz
  mv boost_1_67_0 ${BOOST_BASENAME}-install

fi

# ==============================================================================
# -- Get rpclib and compile it with libc++ -------------------------------------
# ==============================================================================

RPCLIB_BASENAME=rpclib-2.2.1

RPCLIB_INCLUDE=${PWD}/${RPCLIB_BASENAME}-install/include
RPCLIB_LIBPATH=${PWD}/${RPCLIB_BASENAME}-install/lib

if [[ -d "${RPCLIB_BASENAME}-install" ]] ; then
  log "${RPCLIB_BASENAME} already installed."
else
  rm -Rf ${RPCLIB_BASENAME}-source ${RPCLIB_BASENAME}-build

  log "Retrieving rpclib..."

  git clone --depth=1 -b v2.2.1 https://github.com/rpclib/rpclib.git ${RPCLIB_BASENAME}-source

  log "Building rpclib..."

  mkdir -p ${RPCLIB_BASENAME}-build

  pushd ${RPCLIB_BASENAME}-build >/dev/null

  cmake -G "Ninja" \
      -DCMAKE_CXX_FLAGS="-stdlib=libc++ -I${LLVM_INCLUDE} -Wl,-L${LLVM_LIBPATH}" \
      -DCMAKE_INSTALL_PREFIX="../${RPCLIB_BASENAME}-install" \
      ../${RPCLIB_BASENAME}-source

  ninja

  ninja install

  popd >/dev/null

  rm -Rf ${RPCLIB_BASENAME}-source ${RPCLIB_BASENAME}-build

fi

# ==============================================================================
# -- Get GTest and compile it with libc++ --------------------------------------
# ==============================================================================

GTEST_BASENAME=googletest-1.8.0

GTEST_INCLUDE=${PWD}/${GTEST_BASENAME}-install/include
GTEST_LIBPATH=${PWD}/${GTEST_BASENAME}-install/lib

if [[ -d "${GTEST_BASENAME}-install" ]] ; then
  log "${GTEST_BASENAME} already installed."
else
  rm -Rf ${GTEST_BASENAME}-source ${GTEST_BASENAME}-build

  log "Retrieving Google Test..."

  git clone --depth=1 -b release-1.8.0 https://github.com/google/googletest.git ${GTEST_BASENAME}-source

  log "Building Google Test..."

  mkdir -p ${GTEST_BASENAME}-build

  pushd ${GTEST_BASENAME}-build >/dev/null

  cmake -G "Ninja" \
      -DCMAKE_CXX_FLAGS="-stdlib=libc++ -I${LLVM_INCLUDE} -Wl,-L${LLVM_LIBPATH}" \
      -DCMAKE_INSTALL_PREFIX="../${GTEST_BASENAME}-install" \
      ../${GTEST_BASENAME}-source

  ninja

  ninja install

  popd >/dev/null

  rm -Rf ${GTEST_BASENAME}-source ${GTEST_BASENAME}-build

fi

# ==============================================================================
# -- Create CMake config files -------------------------------------------------
# ==============================================================================

log "Generating ${LIBCPP_TOOLCHAIN_FILE}..."

cat >${LIBCPP_TOOLCHAIN_FILE} <<EOL
# Automatically generated by `basename "$0"`

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})

set(CMAKE_CXX_FLAGS "\${CMAKE_CXX_FLAGS} -std=c++17" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "\${CMAKE_CXX_FLAGS} -fPIC -stdlib=libc++ -pthread" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "\${CMAKE_CXX_FLAGS} -I${LLVM_INCLUDE}" CACHE STRING "" FORCE)
set(CMAKE_CXX_LINK_FLAGS "\${CMAKE_CXX_LINK_FLAGS} -L${LLVM_LIBPATH}" CACHE STRING "" FORCE)
set(CMAKE_CXX_LINK_FLAGS "\${CMAKE_CXX_LINK_FLAGS} -lc++ -lc++abi" CACHE STRING "" FORCE)
EOL

log "Generating ${CMAKE_CONFIG_FILE}..."

cat >${CMAKE_CONFIG_FILE} <<EOL
# Automatically generated by `basename "$0"`

set(CARLA_VERSION `git describe --tags --dirty --always`)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

include_directories(
    "${LLVM_INCLUDE}"
    "${BOOST_INCLUDE}"
    "${RPCLIB_INCLUDE}"
    "${GTEST_INCLUDE}")

link_directories(
    "${LLVM_LIBPATH}"
    "${RPCLIB_LIBPATH}"
    "${GTEST_LIBPATH}")

file(GLOB LibCXX_Shared_Libraries "${LLVM_LIBPATH}/libc++*.so*")
install(FILES \${LibCXX_Shared_Libraries} DESTINATION shared)

file(GLOB LibCXX_Static_Libraries "${LLVM_LIBPATH}/libc++*.a")
install(FILES \${LibCXX_Static_Libraries} DESTINATION lib)

install(FILES ${RPCLIB_LIBPATH}/librpc.a DESTINATION lib)
EOL

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

popd >/dev/null

log "Success!"
