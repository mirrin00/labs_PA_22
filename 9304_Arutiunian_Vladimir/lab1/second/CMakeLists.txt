cmake_minimum_required(VERSION 3.23)
project(labs_PA_22/second)

set(SOURCE_DIR "..")

configure_file(${SOURCE_DIR}/matrices.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_CXX_STANDARD 14)

include_directories(${SOURCE_DIR})

add_executable(labs_PA_22
        ${SOURCE_DIR}/matrix/matrix.cpp
        ${SOURCE_DIR}/constant/fork_pid.h
        ${SOURCE_DIR}/constant/filenames.h
        ${SOURCE_DIR}/second/second_task.cpp
        ${SOURCE_DIR}/second/second_task.h)
