cmake_minimum_required(VERSION 3.5)
project(connectome_noMPI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    output/output.txt
    connectome.csv
    edgelist.csv
    main.cpp
    postsynaptic.csv
    synapse.cpp
    synapse.h
    synaptic.csv)

add_executable(connectome_noMPI ${SOURCE_FILES})