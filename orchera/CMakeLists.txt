project(r-type_orchera VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/orchera/include)
set(SHARED_DIR ${CMAKE_SOURCE_DIR}/shared)

set(SRC_FILES
    main.cpp
    scenes/HomeScreen/HomeScreen.cpp
    scenes/Game/Game.cpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio sfml::sfml)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SHARED_DIR})
