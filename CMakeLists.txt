cmake_minimum_required(VERSION 2.8)

project(Route11)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic
    -Wall
    -Wextra
)

add_subdirectory(doc)
add_subdirectory(example)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)
file(GLOB in_hdrs include/route11/*.h)

add_library(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
    ${in_hdrs}
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

install(
    FILES ${hdrs}
    DESTINATION include
)

install(
    FILES ${in_hdrs}
    DESTINATION include/route11
)
