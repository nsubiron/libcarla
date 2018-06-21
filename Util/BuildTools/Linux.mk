default: build

build: _configure
	@cd ${CARLALIB_BUILD_SERVER_FOLDER} && cmake --build .
	@cd ${CARLALIB_BUILD_CLIENT_FOLDER} && cmake --build .

install: build
	@cd ${CARLALIB_BUILD_SERVER_FOLDER} && cmake --build . --target install
	@cd ${CARLALIB_BUILD_CLIENT_FOLDER} && cmake --build . --target install
	@mkdir -p ${CARLAUE4_PLUGIN_DEPS_FOLDER}
	@cp -R ${CARLALIB_INSTALL_SERVER_FOLDER}/* ${CARLAUE4_PLUGIN_DEPS_FOLDER}
	@mkdir -p ${CARLA_PYTHONAPI_DEPS_FOLDER}
	@cp -R ${CARLALIB_INSTALL_CLIENT_FOLDER}/* ${CARLA_PYTHONAPI_DEPS_FOLDER}

setup:
	@${CARLA_BUILD_TOOLS_FOLDER}/Setup.sh

_configure: setup
	@mkdir -p ${CARLALIB_BUILD_SERVER_FOLDER}
	@cd ${CARLALIB_BUILD_SERVER_FOLDER} && cmake \
			-G "Ninja" \
			-DCMAKE_BUILD_TYPE=Server \
			-DCMAKE_TOOLCHAIN_FILE=${LIBCPP_TOOLCHAIN_FILE} \
			-DCMAKE_INSTALL_PREFIX=${CARLALIB_INSTALL_SERVER_FOLDER} \
		  ${CURDIR}
	@mkdir -p ${CARLALIB_BUILD_CLIENT_FOLDER}
	@cd ${CARLALIB_BUILD_CLIENT_FOLDER} && cmake \
			-G "Ninja" \
			-DCMAKE_BUILD_TYPE=Client \
			-DCMAKE_TOOLCHAIN_FILE=${LIBSTDCPP_TOOLCHAIN_FILE} \
			-DCMAKE_INSTALL_PREFIX=${CARLALIB_INSTALL_CLIENT_FOLDER} \
		  ${CURDIR}

check: install
	@LD_LIBRARY_PATH=${CARLALIB_INSTALL_SERVER_FOLDER}/lib ${CARLALIB_INSTALL_SERVER_FOLDER}/test/carlalib_test_debug ${GTEST_ARGS}
	@LD_LIBRARY_PATH=${CARLALIB_INSTALL_SERVER_FOLDER}/lib ${CARLALIB_INSTALL_SERVER_FOLDER}/test/carlalib_test_debug ${GTEST_ARGS}

docs:
	@doxygen
	@echo "Documentation index at ./doxygen/html/index.html"

pretty:
	@${CARLA_BUILD_TOOLS_FOLDER}/Prettify.sh

clean:
	@rm -Rf ${CARLALIB_BUILD_SERVER_FOLDER} ${CARLALIB_BUILD_CLIENT_FOLDER}
	@rm -Rf ${CARLALIB_INSTALL_SERVER_FOLDER} ${CARLALIB_INSTALL_CLIENT_FOLDER}
	@rm -Rf ${CARLAUE4_PLUGIN_DEPS_FOLDER} ${CARLA_PYTHONAPI_DEPS_FOLDER}

CarlaUE4Editor: install
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh

launch:
	@${CARLA_BUILD_TOOLS_FOLDER}/LaunchCarlaUE4.sh

dist: install
	@${CARLA_BUILD_TOOLS_FOLDER}/PackagePythonAPI.sh