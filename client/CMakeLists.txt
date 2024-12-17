project(r-type_client VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_FILES
    main.cpp
    scenes/WaitingRoom/WaitingRoom.cpp
    scenes/Level1/Level1.cpp
    scenes/Level1/Events/PlayerMovement.cpp
    scenes/Level1/Events/PacketHandler.cpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio sfml::sfml)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/client/include)
