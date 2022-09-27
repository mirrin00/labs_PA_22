cmake_minimum_required(VERSION 3.23)
project(labs_PA_22)

set(SOURCE_DIR ".")

configure_file(${SOURCE_DIR}/matrices.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_CXX_STANDARD 14)

include_directories(${SOURCE_DIR})

add_executable(labs_PA_22
        ${SOURCE_DIR}/generateMatrices.cpp)