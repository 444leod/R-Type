cmake_minimum_required(VERSION 3.24)

project(rtype VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if(WIN32)
    set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
endif()

if(UNIX)
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()

find_package(SFML CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

add_subdirectory(client)
add_subdirectory(server)
