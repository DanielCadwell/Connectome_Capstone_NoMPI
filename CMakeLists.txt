cmake_minimum_required(VERSION 3.5)
project(connectome_noMPI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    synapse.cpp
    synapse.h)

add_executable(connectome_noMPI ${SOURCE_FILES})