set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_FILES
    main.cpp
)
list(TRANSFORM SRC_FILES PREPEND "src/")
add_executable(r-type_server ${SRC_FILES})
target_include_directories(r-type_server PUBLIC ${LIB_DIR})
set_target_properties(r-type_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
