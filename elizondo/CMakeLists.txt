cmake_minimum_required(VERSION 3.5)
project(elizondo)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(elizondo
    board_status.hpp
    constants.hpp
    main.cpp
    player.h
    ball.h
    player_controller.hpp
    greedy_player.hpp)