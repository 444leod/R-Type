cmake_minimum_required(VERSION 3.24)

project(rtype VERSION 0.0.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(orchera)
