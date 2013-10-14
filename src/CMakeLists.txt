cmake_minimum_required(VERSION 2.8)

project(route11)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic
    -Wall
    -Wextra
)

add_library(
    ${PROJECT_NAME}

    ../include/Route11.h

    R11AsyncProcess.h
    R11AsyncProcessThread.h
    R11AsyncProcessThread.cpp
    R11DspComponent.h
    R11Process.h
    R11ProcessLoop.h
    R11ProcessPair.h
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC

    ../include
)
