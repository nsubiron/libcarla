default: help

help:
	@less ${CARLA_BUILD_TOOLS_FOLDER}/Linux.mk.help

launch: CarlaLib
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --build --launch

launch-only:
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --launch

docs:
	@doxygen
	@echo "Documentation index at ./doxygen/html/index.html"

clean:
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --clean
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildPythonAPI.sh --clean
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaLib.sh --clean

rebuild: setup
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaLib.sh --rebuild
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildPythonAPI.sh --rebuild
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --rebuild

hard-clean: clean
	@rm -Rf ${CARLA_BUILD_FOLDER}
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --hard-clean

check: PythonAPI
	@${CARLA_BUILD_TOOLS_FOLDER}/Check.sh --all

CarlaUE4Editor: CarlaLib
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --build

.PHONY: PythonAPI
PythonAPI: CarlaLib
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildPythonAPI.sh --rebuild

.PHONY: CarlaLib
CarlaLib: setup
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaLib.sh --server --client

setup:
	@${CARLA_BUILD_TOOLS_FOLDER}/Setup.sh

pretty:
	@${CARLA_BUILD_TOOLS_FOLDER}/Prettify.sh
