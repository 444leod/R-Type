set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

add_executable(r-type_client
        src/main.cpp
        src/scenes/Level1/Level1.cpp
        src/scenes/Level1/Events/PlayerMovement.cpp
        src/scenes/Level1/Events/PlayerMovement.hpp
)
target_include_directories(r-type_client PUBLIC ${LIB_DIR})
target_include_directories(r-type_client PUBLIC ${CMAKE_SOURCE_DIR}/client/include)
target_link_libraries(r-type_client PRIVATE sfml-graphics sfml-window sfml-system)
set_target_properties(r-type_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
