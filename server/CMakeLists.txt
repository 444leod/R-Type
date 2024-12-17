project(r-type_server VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/server/include)
set(SRC_FILES
    main.cpp
    scenes/WaitingRoom/WaitingRoom.cpp
    scenes/Level1/Level1.cpp
    scenes/Level1/Events/PacketHandler.cpp
    scenes/Level1/Events/PlayerMovement.cpp
)
list(TRANSFORM SRC_FILES PREPEND "src/")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio sfml::sfml)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
