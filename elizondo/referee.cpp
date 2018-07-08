#ifndef REFEREE
#define REFEREE

#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "board_status.hpp"
#include "greedy_player.hpp"
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
        greedy_player* teamA;
        greedy_player* teamB;
        LogicalBoard* board;

        Referee(
            int columns,
            int rows,
            int steps,
            vector < player > teamA,
            vector < player > teamB,
            vector < double > loadsA,
            vector < double > loadsB
        ) {
            //Set variables
            this->columns = columns;
            this->rows = rows;
            this->steps = steps;

            //Create Greedy Team A
            this->teamA = new greedy_player(
                this->columns,
                this->rows,
                this->steps,
                IZQUIERDA,
                teamA,
                teamB,
                loadsA
            );

            //Create Greedy Team B
            this->teamB = new greedy_player(
                this->columns,
                this->rows,
                this->steps,
                DERECHA,
                teamB,
                teamA,
                loadsB
            );

            //Create logical board
            LogicalBoard* logical_board = new LogicalBoard(
                this->columns,
                this->rows,
                teamA,
                teamB
            );
            this->board = logical_board;
        }

        string runPlay(string startingTeam) {
            vector <player_status> positionsA;
            this->teamA->starting_positions(positionsA);
            // for (auto i = positionsA.begin(); i != positionsA.end(); ++i)
                // cout << "A -> ID " << (*i).id << " (" << (*i).i << ',' << (*i).j << ')' << endl;

            vector <player_status> positionsB;
            this->teamB->starting_positions(positionsB);
            // for (auto i = positionsB.begin(); i != positionsB.end(); ++i)
                // cout << "B -> ID " << (*i).id << " (" << (*i).i << ',' << (*i).j << ')' << endl;
            
            board->reset(positionsA, positionsB, IZQUIERDA);

            //Itero sobre todos los steps del partido
            for (int i = 0; i < steps; i++) {
                board_status* board_state = board->getCurrentState();

                vector<player_move> madeMovesA;
                this->teamA->make_move(*(board_state), madeMovesA);
                // for (auto i = madeMovesA.begin(); i != madeMovesA.end(); ++i)
                    // cout << "A -> ID " << (*i).player_id << " TYPE " << (*i).move_type << " DIR " << (*i).dir << " STEPS " << (*i).steps << endl;

                vector<player_move> madeMovesB;
                this->teamB->make_move(*(board_state), madeMovesB);
                // for (auto i = madeMovesB.begin(); i != madeMovesB.end(); ++i)
                    // cout << "B -> ID " << (*i).player_id << " TYPE " << (*i).move_type << " DIR " << (*i).dir << " STEPS " << (*i).steps << endl;

                string starter = this->board->makeMove(madeMovesA, madeMovesB);
                
                //Si alguno hizo un gol
                if (starter != TIE) {
                    board->startingPositions(positionsA, positionsB, starter);
                }
            }

            return board->winner();
        }
    };

#endif //REFEREE