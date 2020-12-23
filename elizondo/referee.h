#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ai_greedy.hpp"
#include "ball.h"
#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.hpp"
#include "player.h"

struct Referee
{
    int columns;
    int rows;
    int steps;
    std::unique_ptr<AI> teamA;
    std::unique_ptr<AI> teamB;
    LogicalBoard board;

    Referee(
        int pColumns,
        int pRows,
        int pSteps,
        std::vector<Player> pTeamA,
        std::vector<Player> pTeamB,
        std::vector<double> pLoadsA,
        std::vector<double> pLoadsB)
        : columns(pColumns)
        , rows(pRows)
        , steps(pSteps)
        , teamA(std::make_unique<greedy_player>(pColumns, pRows, pSteps, IZQUIERDA, pTeamA, pTeamB, pLoadsA))
        , teamB(std::make_unique<greedy_player>(pColumns, pRows, pSteps, IZQUIERDA, pTeamB, pTeamA, pLoadsB))
        , board(columns, rows, pTeamA, pTeamB)
    {
    }

    ~Referee()
    {
    }

    int getScore(std::string team) const
    {
        if (team == IZQUIERDA)
            return board.get_scoreA();

        return board.get_scoreB();
    }

    std::string runPlay(std::string startingTeam)
    {
        std::vector<player_status> positionsA = teamA->starting_positions();
        std::vector<player_status> positionsB = teamB->starting_positions();

        board.reset(positionsA, positionsB, startingTeam);

        //Itero sobre todos los steps del partido
        for (int i = 0; i < steps; i++)
        {
            board_status board_state = board.getCurrentState();

            std::vector<player_move> madeMovesA = teamA->make_move(board_state);
            std::vector<player_move> madeMovesB = teamB->make_move(board_state);

            std::string starter = board.makeMove(madeMovesA, madeMovesB);

            //Si alguno hizo un gol
            if (starter != TIE)
            {
                board.startingPositions(positionsA, positionsB, starter);
            }
        }

        return board.winner();
    }
};
