CMAKE_FLAGS+=-G "Ninja"
CMAKE_FLAGS+=-DCMAKE_TOOLCHAIN_FILE=${LIBCPP_TOOLCHAIN_FILE}
CMAKE_FLAGS+=-DCMAKE_INSTALL_PREFIX=${CARLALIB_INSTALL_FOLDER}

export LD_LIBRARY_PATH=${CARLALIB_INSTALL_FOLDER}/shared

default: build

check: check_debug

build: _call_cmake
	@cd ${CARLALIB_BUILD_FOLDER} && cmake --build .

install: build
	@cd ${CARLALIB_BUILD_FOLDER} && cmake --build . --target install

_call_cmake:
	@${CARLA_BUILD_TOOLS_FOLDER}/Setup.sh
	@mkdir -p ${CARLALIB_BUILD_FOLDER}
	@cd ${CARLALIB_BUILD_FOLDER} && cmake ${CMAKE_FLAGS} ${CURDIR}

check_release: install
	@${CARLALIB_INSTALL_FOLDER}/test/carlalib_test_debug ${GTEST_ARGS}

check_debug: install
	@${CARLALIB_INSTALL_FOLDER}/test/carlalib_test_debug ${GTEST_ARGS}

docs:
	@doxygen
	@echo "Documentation index at ./doxygen/html/index.html"

pretty:
	@${CARLA_BUILD_TOOLS_FOLDER}/Prettify.sh

clean:
	@rm -Rf ${CARLALIB_BUILD_FOLDER}
