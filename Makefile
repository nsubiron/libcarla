BUILD_FOLDER=$(CURDIR)/build/carlalib-build
INSTALL_FOLDER=$(CURDIR)/install
TOOLCHAIN=$(CURDIR)/build/ToolChain.cmake

CMAKE_FLAGS+=-G "Ninja"
CMAKE_FLAGS+=-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN)
CMAKE_FLAGS+=-DCMAKE_INSTALL_PREFIX=$(INSTALL_FOLDER)

default: build

check: check_debug

build: _call_cmake
	@cd $(BUILD_FOLDER) && cmake --build .

install: build
	@cd $(BUILD_FOLDER) && cmake --build . --target install

_call_cmake:
	@./Setup.sh
	@mkdir -p $(BUILD_FOLDER)
	@cd $(BUILD_FOLDER) && cmake $(CMAKE_FLAGS) $(CURDIR)

check_release: install
	@LD_LIBRARY_PATH=$(INSTALL_FOLDER)/shared $(INSTALL_FOLDER)/test/carlalib_test_debug

check_debug: install
	@LD_LIBRARY_PATH=$(INSTALL_FOLDER)/shared $(INSTALL_FOLDER)/test/carlalib_test_debug

docs:
	@doxygen
	@echo "Documentation index at ./doxygen/html/index.html"

pretty:
	@./Prettify.sh

clean:
	@rm -Rf $(BUILD_FOLDER)
