#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "auxiliars.hpp"
#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.hpp"

std::random_device rd;
std::mt19937 generator(rd());

class tournament_player
{

    int columns, rows;
    double MAX_DIST;
    std::string team, side;
    std::vector<std::pair<int, int>> own_goal;
    std::vector<std::pair<int, int>> opponnent_goal;
    std::vector<Player> players;
    std::vector<Player> opponnents;
    std::vector<double> loads;

public:

    tournament_player() = default;

    ~tournament_player() = default;

    tournament_player(
        int pColumns,
        int pRows,
        int,
        string pSide,
        const vector<Player>& pPlayers,
        const vector<Player>& pOpponnents
    )
    : columns(pColumns)
    , rows(pRows)
    , side(pSide)
    , players(pPlayers)
    , opponnents(pOpponnents)
    {
        for(int i = 0; i < 3; i++)
        {
            players[i].id = i;
        }

        for(int i = 0; i < 3; i++)
        {
            opponnents[i].id = i;
        }

        get_goal_positions();
        loads = getLoads();
        MAX_DIST = distance(0, 0, rows, columns);
    }

    tournament_player(
        int pColumns,
        int pRows,
        int,
        string pSide,
        const vector<Player>& pPlayers,
        const vector<Player>& pOpponnents,
        vector<double>& pLoads
    )
    : columns(pColumns)
    , rows(pRows)
    , side(pSide)
    , players(pPlayers)
    , opponnents(pOpponnents)
    , loads(pLoads)
    {
        for(int i = 0; i < 3; i++)
        {
            players[i].id = i;
        }

        for(int i = 0; i < 3; i++)
        {
            opponnents[i].id = i;
        }

        get_goal_positions();

        MAX_DIST = distance(0, 0, rows, columns);
    }

    vector<double> getLoads()
    {
        return {0.95, 0.93, 0.39, 0.78, 0.33, 0.15, 0.56, 0.26, 0.62, 0.65};
    }

    void get_goal_positions()
    {
        const int mid_row = rows / 2;
        int column_own_goal = columns;
        int column_opponnent_goal = -1;

        if (side == IZQUIERDA)
        {
            column_own_goal = -1;
            column_opponnent_goal = columns;
        }

        for (int i = -1; i < 2; i++)
        {
            own_goal.emplace_back(mid_row + i, column_own_goal);
            opponnent_goal.emplace_back(mid_row + i, column_opponnent_goal);
        }
    }

    void starting_positions(vector<player_status>& positions)
    {
        int column = columns - 1;

        if (side == IZQUIERDA)
        {
            column = 0;
        }

        for (int i = 0; i < 3; i++)
        {
            positions.emplace_back(i, i, column, false);
        }
    }

    void make_move(const board_status& current_board, vector<player_move>& made_moves)
    {
        made_moves.clear();
        made_moves.resize(3);

        board_status board = current_board;

        if (side == "DERECHA")
        {
            board.team = current_board.oponent_team;
            board.oponent_team = current_board.team;
        }
        
        vector<int> found_move = search_move(board);
        update_moves(board, made_moves, found_move[0], found_move[1], found_move[2], found_move[3], found_move[4]);
    }

    void finish(string&)
    {
    }

private:

    vector<int> search_move(const board_status& current_board)
    {
        double max_rank = -999999;
        double current_rank;
        std::vector<int> result (5,0);

        for (int i = 0; i < moves.size(); i++) {
            if (inside_board(current_board.team[0], i, opponnent_goal, rows, columns))
            {
                for (int j = 0; j < moves.size(); j++)
                {
                    if (inside_board(current_board.team[1], j, opponnent_goal, rows, columns))
                    {
                        for (int k = 0; k < moves.size(); k++)
                        {
                            if (inside_board(current_board.team[2], k, opponnent_goal, rows, columns))
                            {
                                std::vector<int> player_moves { i, j, k };
                                for (int jugador = 0; jugador < 3; jugador++)
                                {
                                    vector<int> player_moves_for_pass { i, j, k };
                                    player_moves_for_pass[jugador] = 0;
                                    if (current_board.team[jugador].in_posetion && player_moves[jugador] != 0 && in_different_positions(current_board.team, player_moves_for_pass))
                                    {
                                        int max_steps = calculate_max_steps(current_board.team[jugador], player_moves[jugador], rows, columns, opponnent_goal);
                                        for (int steps = 1; steps <= max_steps; steps++)
                                        {
                                            current_rank = evaluate_board(current_board, i, j, k, jugador, steps);
                                            if (current_rank > max_rank)
                                            {
                                                max_rank = current_rank;
                                                result = {i,j,k,jugador,steps};
                                            }
                                        }
                                    }
                                }
                                if (in_different_positions(current_board.team, player_moves))
                                {
                                    current_rank = evaluate_board(current_board, i, j, k);
                                    if (current_rank > max_rank)
                                    {
                                        max_rank = current_rank;
                                        result = {i,j,k,0,0};
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    void update_moves(const board_status& current_board, vector<player_move>& made_moves, int p0, int p1, int p2, int player_with_ball = 0, int steps = 0)
    {
        //si nadie patea todos tienen step 0
        vector<int> steps_players(3, 0);
        //si algun jugador pateo, actualizo su step
        steps_players[player_with_ball] = steps;

        vector<int> player_moves = {p0,p1,p2};

        for (int i = 0; i < 3; i++)
        {
            made_moves[i].player_id = current_board.team[i].id;
            made_moves[i].move_type = steps_players[i] == 0 ? MOVIMIENTO : PASE;
            made_moves[i].dir = player_moves[i];
            made_moves[i].steps = steps_players[i];
        }
    }

    double evaluate_board(const board_status& current_board, int i, int j, int k, int jugador = 0, int steps = 0)
    {
        board_status updated_board = update_board(current_board, i, j, k, jugador, steps);

        double result = 0;

        for (auto goal : opponnent_goal)
        {
            if (get_ball_position(updated_board) == goal)
            {
                return 999999;
            }
        }

        for (auto goal : own_goal)
        {
            if (get_ball_position(updated_board) == goal)
            {
                return 0;
            }
        }

        if (who_has_the_ball(updated_board) == "GREEDY")
        {
            for (auto p : updated_board.team)
            {
                result += MAX_DIST - distance_player_opponnent_goal(p, opponnent_goal) * loads[0]; //distancia al arco contrario
                result += distance_player_closest_opponnent(updated_board, p) * loads[1]; //distancia al oponente mas cercano
            }
            result += dispersion(updated_board.team) * loads[2];
            result /= 7;
        } 

        if (who_has_the_ball(updated_board) == "OPPONNENT")
        {
            for (auto p : updated_board.team) {
                result += MAX_DIST - distance_player_ball(updated_board, p) * loads[3]; //distancia a la pelota
                result += MAX_DIST - distance_player_closest_opponnent(updated_board, p) * loads[4]; //distancia al oponente mas cercano
            }
            result += dispersion(updated_board.team) * loads[5];
            result /= 7;
        }

        if (who_has_the_ball(updated_board) == "FREE")
        {
            for (auto p : updated_board.team) {
                result += MAX_DIST - distance_player_ball(updated_board, p) * loads[6]; //distancia a la pelota
                result += MAX_DIST - distance_player_opponnent_goal(p, opponnent_goal) * loads[7]; //distancia al arco contrario
            }
            
            for (auto opp_p : updated_board.oponent_team)
            {
                result += distance_player_ball(updated_board, opp_p) * loads[8]; //distancia a la pelota del contrario
            }

            result += dispersion(updated_board.team) * loads[9];
            result /= 10;
        }

        return result;
    }

    board_status update_board(const board_status& current_board, int i, int j, int k, int jugador, int steps)
    {
        board_status updated_board;

        updated_board = current_board;

        // si nadie tenia la pelota
        if (updated_board.ball.is_free)
        {
            // si venia moviendose actualizo su posicion
            if (updated_board.ball.steps > 0)
            {
                updated_board.ball.i += 2*moves[updated_board.ball.dir].first;
                updated_board.ball.j += 2*moves[updated_board.ball.dir].second;
            }
        }

        updated_board.team[0].i += moves[i].first;
        updated_board.team[0].j += moves[i].second;
        updated_board.team[1].i += moves[j].first;
        updated_board.team[1].j += moves[j].second;
        updated_board.team[2].i += moves[k].first;
        updated_board.team[2].j += moves[k].second;

        vector<int> player_moves { i, j, k };
        for (auto p : updated_board.team)
        {
            if (p.id == jugador && steps > 0)
            {
                p.in_posetion = false;
                updated_board.ball.is_free = true;
                updated_board.ball.steps = steps;
                updated_board.ball.dir = player_moves[jugador];
                updated_board.ball.i += 2*moves[player_moves[jugador]].first;
                updated_board.ball.j += 2*moves[player_moves[jugador]].second;
                p.i -= moves[player_moves[jugador]].first;
                p.j -= moves[player_moves[jugador]].second;
            }
        }
        
        for (int h = 0; h < 3; h++)
        {
            if (current_board.ball.is_free 
                && updated_board.ball.is_free 
                && updated_board.team[h].i == updated_board.ball.i 
                && updated_board.team[h].j == updated_board.ball.j 
                && player_moves[h] == 0)
            {
                updated_board.ball.is_free = false;
                updated_board.team[h].in_posetion = true;
            }
        }

        for (int h = 0; h < 3; h++)
        {
            if (current_board.ball.is_free 
                && updated_board.ball.is_free 
                && updated_board.oponent_team[h].i == updated_board.ball.i 
                && updated_board.oponent_team[h].j == updated_board.ball.j 
                && player_moves[h] == 0)
            {
                updated_board.ball.is_free = false;
                updated_board.oponent_team[h].in_posetion = true;
            }
        }

        return updated_board;
    }
};