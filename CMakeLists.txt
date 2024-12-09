cmake_minimum_required(VERSION 3.18)

project(rtype VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

add_subdirectory(client)
add_subdirectory(server)
