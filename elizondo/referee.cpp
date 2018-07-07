#ifndef REFEREE
#define REFEREE

#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "board_status.hpp"
#include "constants.hpp"
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
        LogicalBoard* board;

        Referee(
            int columns,
            int rows,
            int steps,
            vector < player > teamA,
            vector < player > teamB
        ) {
            //Set variables
            this->columns = columns;
            this->rows = rows;
            this->steps = steps;
            this->teamA = teamA;
            this->teamB = teamB;

            //Create logical board
            LogicalBoard* logical_board = new LogicalBoard(
                this->columns,
                this->rows,
                this->teamA,
                this->teamB
            );
            this->board = logical_board;
        }

        string runPlay(string startingTeam) {
            return IZQUIERDA;
        }
    };

#endif //REFEREE