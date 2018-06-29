#ifndef REFEREE
#define REFERE

#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "board_status.hpp"
#include "logical_board.cpp"
#include "player.h"
#include "ball.h"

using namespace std;

class Referee {

    public:
    
        int columns;
        int rows;
        int steps;
        vector < player > teamA;
        vector < player > teamB;
        vector < double > quitesA;
        vector < double > quitesB;
        LogicalBoard* board;

        Referee(
            int columns,
            int rows,
            int steps,
            vector < player > teamA,
            vector < player > teamB,
            vector < double > quitesA,
            vector < double > quitesB
        ) {
            //Set variables
            this->columns = columns;
            this->rows = rows;
            this->steps = steps;

            //Set players Team A quites
            vector < player > _teamA;
            for(auto p : teamA) {
                player* aux = new player(p.id, quitesA[p.id]);
                _teamA.push_back(*aux);
            }
            this->teamA = _teamA;

            //Set players Team B with quites
            vector < player > _teamB;
            for(auto p : teamB) {
                player* aux = new player(p.id, quitesB[p.id]);
                _teamB.push_back(*aux);
            }
            this->teamB = _teamB;
            
            //Create logical board
            LogicalBoard* logical_board = new LogicalBoard(
                this->columns,
                this->rows,
                this->teamA,
                this->teamB
            );
            this->board = logical_board;
        }
};

#endif //REFEREE