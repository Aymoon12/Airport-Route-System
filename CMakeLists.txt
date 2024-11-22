cmake_minimum_required(VERSION 3.27)
project(Airport_Route_System)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Airport_Route_System
        AVL.cpp
        AVL.h
        Core_Functions.cpp
        Core_Functions.h
        Graph.cpp
        Graph.h
        main.cpp
        Queue.cpp
        Queue.h
        Stack.cpp
        Stack.h)
