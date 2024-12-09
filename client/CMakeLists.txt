set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_FILES
    main.cpp
    scenes/Game/Game.cpp
    scenes/Game/Events/PlayerMovement.cpp
    scenes/Game/Events/PlayerMovement.hpp
)

list(TRANSFORM SRC_FILES PREPEND "src/")
add_executable(r-type_client ${SRC_FILES})
target_include_directories(r-type_client PUBLIC ${LIB_DIR})
target_link_libraries(r-type_client PRIVATE sfml-graphics sfml-window sfml-system)
set_target_properties(r-type_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
