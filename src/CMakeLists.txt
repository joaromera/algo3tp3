cmake_minimum_required(VERSION 3.5)
project(src)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(src
        jugador.cpp
        jugador.h
        main.cpp
        pelota.cpp
        pelota.h
        posicion.cpp
        posicion.h
	partido.cpp
	partido.h)
