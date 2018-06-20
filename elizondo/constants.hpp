#ifndef CONSTANTS
#define CONSTANTS

#include <string>
#include <vector>
#include <utility>

const std::string IZQUIERDA = "IZQUIERDA";

const std::string MOVIMIENTO = "MOVIMIENTO";
const std::string PASE = "PASE";
const std::string IN_POSETION = "CON_PELOTA";
const std::string FREE_PLAYER = "SIN_PELOTA";
const std::string EXIT = "SALIR";
const std::string DONE = "LISTO";

const std::string LOSE = "PERDISTE";
const std::string WIN = "GANASTE";
const std::string TIE = "EMPATARON";

const std::vector<move> playerMoves = {
    {0, -1, -1}, 
    {1, -1, 0}, 
    {2, -1, 1}, 
    {3, 0, 1}, 
    {4, 1, 1}, 
    {5, 1, 0}, 
    {6, 1, -1}, 
    {7, 0, -1}, 
    {8, 0, 0}
};

const std::vector < std::pair < int, int > > moves = { 
    std::make_pair(0,0), 
    std::make_pair(-1,-1), 
    std::make_pair(-1,0), 
    std::make_pair(-1,1), 
    std::make_pair(0,1), 
    std::make_pair(1,1), 
    std::make_pair(1,0), 
    std::make_pair(1,-1), 
    std::make_pair(0,-1) 
};


#endif //CONSTANTS