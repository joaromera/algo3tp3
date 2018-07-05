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

        // string runPlay(string startingTeam) {
            // team_A.starting_positions = team_A.startGame(board->columns, board->rows, board->steps, IZQUIERDA, team_A.players, team_B.players);
            // team_B.starting_positions = team_B.startGame(board->columns, board->rows, board->steps, DERECHA, team_B.players, team_A.players);

            // board->reset(team_A.starting_positions, team_B.starting_positions, startingTeam);

            // //Itero sobre todos los steps del partido
            // for (int i = 0; i < steps; i++) {
                // board_state_A = board->getState();
                // board_state_B = board->getState();
                // moves_A = team_A.move(board_state_A);
                // moves_B = team_B.move(board_state_B);

                // string starter = board->makeMove(moves_A, moves_B);

                // //Si alguno hizo un gol
                // if (starter != TIE) {
                    // board->startingPositions(team_A.starting_positions, team_B.starting_positions, starter);
                // }
            // }

            // string winner = board->winner();
            // team_A.finished(winner)
            // team_B.finished(winner)

            // return winner
        // }
    };

#endif //REFEREE