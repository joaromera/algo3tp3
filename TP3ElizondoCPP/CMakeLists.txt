cmake_minimum_required(VERSION 3.5)
project(TP3ElizondoCPP)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(TP3ElizondoCPP
    elizondo.cpp
    referee.h
    logicalBoard.h
    boardStatus.hpp
    constants.hpp)
