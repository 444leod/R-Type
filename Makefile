##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

BUILD_DIR = build
CLEAN_TARGETS = $(BUILD_DIR) CMakeCache.txt CMakeFiles documentation/doxygen/html

.PHONY: all deps configure build install clean conan docs

all: install

conan:
	chmod +x ./scripts/install_conan.sh
	./scripts/install_conan.sh

conan_ci:
	chmod +x ./scripts/install_conan_ci.sh
	./scripts/install_conan_ci.sh

deps:
	conan profile detect --force
	conan install . --output-folder=$(BUILD_DIR) --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True


deps_windows_release:
	conan profile detect --force
	conan install . --output-folder=$(BUILD_DIR) --build=missing -c tools.system.package_manager:mode=install --profile=profiles/windows

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(BUILD_DIR)/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

build: configure
	cmake --build $(BUILD_DIR)

install: build
	cmake --install $(BUILD_DIR)

clean:
	rm -rf $(CLEAN_TARGETS)

docs:
	./documentation/doxygen/generator
	doxygen ./documentation/doxygen/Doxyfile
	xdg-open documentation/doxygen/html/index.html
