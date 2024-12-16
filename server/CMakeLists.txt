project(r-type_server VERSION 0.0.1)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_FILES
    main.cpp
)
list(TRANSFORM SRC_FILES PREPEND "src/")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR})
