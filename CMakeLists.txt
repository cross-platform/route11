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

file(GLOB hdrs include/*.h)
file(GLOB in_hdrs include/route11/*.h)

add_custom_target(srcs SOURCES ${hdrs} ${in_hdrs})

install(
    FILES ${hdrs}
    DESTINATION include
)

install(
    FILES ${in_hdrs}
    DESTINATION include/route11
)
