#pragma once

#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "ai_greedy.hpp"
#include "ball.h"
#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.hpp"
#include "player.h"

using namespace std;

class Referee {

    public:

        int columns;
        int rows;
        int steps;
        AI* teamA;
        AI* teamB;
        LogicalBoard* board;

        ~Referee() {
            delete teamA;
            delete teamB;
            delete board;
        }

        Referee(
            int columns,
            int rows,
            int steps,
            vector < Player > teamA,
            vector < Player > teamB,
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

        int getScore(string team) {
            if (team == IZQUIERDA)
                return this->board->get_scoreA();
            return this->board->get_scoreB();
        }

        string runPlay(string startingTeam) {
            // string fileName = "REFEREE.log";
            // ofstream fs;
            // fs.open(fileName, fstream::out);
            
            vector <player_status> positionsA;
            this->teamA->starting_positions(positionsA);
            // for (auto i = positionsA.begin(); i != positionsA.end(); ++i)
            //     fs << "A -> ID " << (*i).id << " (" << (*i).i << ',' << (*i).j << ')' << endl;

            vector <player_status> positionsB;
            this->teamB->starting_positions(positionsB);
            // for (auto i = positionsB.begin(); i != positionsB.end(); ++i)
            //     fs << "B -> ID " << (*i).id << " (" << (*i).i << ',' << (*i).j << ')' << endl;
            
            board->reset(positionsA, positionsB, startingTeam);

            // fs << "STARTING MATCH" << endl;

            //Itero sobre todos los steps del partido
            for (int i = 0; i < steps; i++) {
                board_status board_state = board->getCurrentState();
                // for (int j = 0; j < 3; j++) {
                //     fs << "index: "<< j << " id: " << board_state->team[j].id << " i: " << board_state->team[j].i << " j: " << board_state->team[j].j << " ball: " << board_state->team[j].in_posetion << endl;
                // }
                // for (int j = 0; j < 3; j++) {
                //     fs << "index: "<< j << " id: " << board_state->oponent_team[j].id << " i: " << board_state->oponent_team[j].i << " j: " << board_state->oponent_team[j].j << " ball: " << board_state->oponent_team[j].in_posetion << endl;
                // }

                vector<player_move> madeMovesA;
                this->teamA->make_move(board_state, madeMovesA);
                // for (auto i = madeMovesA.begin(); i != madeMovesA.end(); ++i)
                //     fs << "A -> ID " << (*i).player_id << " TYPE " << (*i).move_type << " DIR " << (*i).dir << " STEPS " << (*i).steps << endl;

                vector<player_move> madeMovesB;
                this->teamB->make_move(board_state, madeMovesB);
                // for (auto i = madeMovesB.begin(); i != madeMovesB.end(); ++i) {
                    // (*i).dir = 0;
                    // (*i).move_type = "MOVIMIENTO";
                    // (*i).steps = 0;
                    // fs << "B -> ID " << (*i).player_id << " TYPE " << (*i).move_type << " DIR " << (*i).dir << " STEPS " << (*i).steps << endl;

                // }
                
                // if (board->free_ball != nullptr) {
                //     fs << "ball i: " << this->board->free_ball->i << " j: " << this->board->free_ball->j << endl;
                // }

                string starter = this->board->makeMove(madeMovesA, madeMovesB);
                
                //Si alguno hizo un gol
                if (starter != TIE) {
                    // fs << starter << endl;
                    board->startingPositions(positionsA, positionsB, starter);
                }
            }
            // cout << "ENDING MATCH" << endl;
            // fs << board->winner() <<endl;
            // fs.close();

            return board->winner();
        }
};
