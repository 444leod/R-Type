project(r-type_client VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/client/include)
set(SHARED_DIR ${CMAKE_SOURCE_DIR}/shared)

set(SRC_FILES
    main.cpp
    scenes/WaitingRoom/WaitingRoom.cpp
    scenes/WaitingRoom/Modules/PacketHandlerSceneModule.cpp
    scenes/Level1/Level1.cpp
    scenes/Level1/Modules/PacketHandlerSceneModule.cpp
    scenes/Win/Win.cpp
    scenes/Lose/Lose.cpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio sfml::sfml)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${SHARED_DIR})
