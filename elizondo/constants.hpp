#pragma once

#include <string>
#include <utility>
#include <vector>

#include "board_status.hpp"

static const std::string IZQUIERDA = "IZQUIERDA";
static const std::string DERECHA = "DERECHA";
static const std::string MOVIMIENTO = "MOVIMIENTO";
static const std::string PASE = "PASE";
static const std::string IN_POSETION = "CON_PELOTA";
static const std::string FREE_PLAYER = "SIN_PELOTA";
static const std::string EXIT = "SALIR";
static const std::string DONE = "LISTO";
static const std::string LOSE = "PERDISTE";
static const std::string WIN = "GANASTE";
static const std::string TIE = "EMPATARON";

static const std::vector<move> _moves = {
    { 0, -1, -1 },
    { 1, -1, 0 },
    { 2, -1, 1 },
    { 3, 0, 1 },
    { 4, 1, 1 },
    { 5, 1, 0 },
    { 6, 1, -1 },
    { 7, 0, -1 },
    { 8, 0, 0 }
};

static const std::vector<std::pair<int, int>> moves = {
    {  0,  0 },
    { -1, -1 },
    { -1,  0 },
    { -1,  1 },
    {  0,  1 },
    {  1,  1 },
    {  1,  0 },
    {  1, -1 },
    {  0, -1 }
};
