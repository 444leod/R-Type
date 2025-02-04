project(r-type_client VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/client/include)
set(SHARED_DIR ${CMAKE_SOURCE_DIR}/shared)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/scenes/WaitingRoom/WaitingRoom.cpp
        src/scenes/WaitingRoom/Modules/PacketHandlerSceneModule.cpp
        src/scenes/Level1/Level1.cpp
        src/scenes/Level1/Modules/PacketHandlerSceneModule.cpp
        src/scenes/EndScreen/EndScreen.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio sfml::sfml)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SHARED_DIR})
