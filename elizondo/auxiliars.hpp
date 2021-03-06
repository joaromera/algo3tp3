#pragma once

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "board_status.hpp"
#include "constants.hpp"

double distance(int from_i, int from_j, int to_i, int to_j)
{
    const double x = static_cast<double>(from_i) - static_cast<double>(to_i);
    const double y = static_cast<double>(from_j) - static_cast<double>(to_j);
    return std::sqrt(pow(x, 2) + pow(y, 2));
}

double distance_player_opponnent_goal(const player_status &player, const std::vector<std::pair<int, int>> &opponnent_goal)
{
    double min_distance = 999999;
    for (const auto &goal : opponnent_goal)
    {
        const double aux = distance(player.i, player.j, goal.first, goal.second);
        if (aux < min_distance)
        {
            min_distance = aux;
        }
    }
    return min_distance;
}

double dispersion(const std::vector<player_status> &team)
{
    return (distance(team[0].i, team[0].j, team[1].i, team[1].j) +
            distance(team[0].i, team[0].j, team[2].i, team[2].j) +
            distance(team[1].i, team[1].j, team[2].i, team[2].j)) / 3;
}

std::string who_has_the_ball(const board_status &current_board)
{
    for (const auto &p : current_board.team)
    {
        if (p.in_posetion)
        {
            return "GREEDY";
        }
    }

    for (const auto &p : current_board.oponent_team)
    {
        if (p.in_posetion)
        {
            return "OPPONNENT";
        }
    }

    return "FREE";
}

std::pair<int, int> get_ball_position(const board_status &current_board)
{
    if (current_board.ball.is_free)
    {
        return { current_board.ball.i, current_board.ball.j };
    }
    else
    {
        for (const auto &p : current_board.team)
        {
            if (p.in_posetion)
            {
                return { p.i, p.j };
            }
        }

        for (const auto &p : current_board.oponent_team)
        {
            if (p.in_posetion)
            {
                return { p.i, p.j };
            }
        }
    }
    return { -1, -1 };
}

int distance_player_ball(const board_status &current_board, const player_status &player)
{
    const auto [ball_i, ball_j] = get_ball_position(current_board);

    const int di1 = distance(player.i, player.j, ball_i, ball_j);
    const int di2 = distance(player.i, player.j, ball_i, ball_j);
    const int di3 = distance(player.i, player.j, ball_i, ball_j);

    return std::min(di1, std::min(di2, di3));
}

double distance_player_closest_opponnent(const board_status &current_board, const player_status &player)
{
    double min_distance = 999999;
    for (const auto &opponnent : current_board.oponent_team)
    {
        const double aux = distance(player.i, player.j, opponnent.i, opponnent.j);

        if (aux < min_distance)
        {
            min_distance = aux;
        }
    }
    return min_distance;
}

bool is_in_same_rect(int pos_i, int pos_j, int goal_row, int a, const std::vector<std::pair<int, int>> &opponnent_goal)
{
    // y = a*x => y - a*x = 0, a puede ser 1 o -1
    return opponnent_goal[goal_row].first - a * opponnent_goal[goal_row].second == pos_i - a * pos_j;
}

bool closer_to_opponent_goal(int player_j, int opp_player_j, const std::vector<std::pair<int, int>> &opponnent_goal)
{
    // dice si el jugador del equipo greedy esta entre el oponente y su arco
    return abs(opponnent_goal[0].second - player_j) < abs(opponnent_goal[0].second - opp_player_j);
}

bool opponents_blocking_goal(const board_status &current_board, player_status player, int goal_row, int a, std::vector<std::pair<int, int>> opponnent_goal)
{
    bool is_in_between = false;
    for (const auto &opp_p : current_board.oponent_team)
    {
        is_in_between |= (is_in_same_rect(opp_p.i, opp_p.j, goal_row, a, opponnent_goal)
                          && !closer_to_opponent_goal(player.j, opp_p.j, opponnent_goal));
    }
    return is_in_between;
}

double distance_ball_opponnent_goal(const board_status &current_board, const std::vector<std::pair<int, int>> &opponnent_goal)
{
    auto [ball_i, ball_j] = get_ball_position(current_board);

    const double di1 = distance(ball_i, ball_j, opponnent_goal[0].first, opponnent_goal[0].second);
    const double di2 = distance(ball_i, ball_j, opponnent_goal[1].first, opponnent_goal[1].second);
    const double di3 = distance(ball_i, ball_j, opponnent_goal[2].first, opponnent_goal[2].second);

    return std::min(di1, std::min(di2, di3));
}

bool moving_towards_goal(const ball_status &ball, int a, const std::vector<std::pair<int, int>> &opponnent_goal)
{
    if (opponnent_goal[0].second == -1)
    {
        // arco rival en columna -1
        return (a == -1 && ball.dir == 1) || (a == 0 && ball.dir == 8) || (a == 1 && ball.dir == 7);
    }
    // arco rival en ultima columna
    return (a == -1 && ball.dir == 5) || (a == 0 && ball.dir == 4) || (a == 1 && ball.dir == 3);
}

bool has_kicked_to_goal(const board_status &board, std::vector<std::pair<int, int>> &opponnent_goal)
{
    const ball_status ball = board.ball;
    bool has_goal_direction = false;

    for (int goal_row = 0; goal_row < 3; goal_row++)
    {
        has_goal_direction |= (is_in_same_rect(ball.i, ball.j, goal_row, -1, opponnent_goal)
                               && moving_towards_goal(ball, -1, opponnent_goal));

        has_goal_direction |= (is_in_same_rect(ball.i, ball.j, goal_row, 0, opponnent_goal)
                               && moving_towards_goal(ball, 0, opponnent_goal));

        has_goal_direction |= (is_in_same_rect(ball.i, ball.j, goal_row, 1, opponnent_goal)
                               && moving_towards_goal(ball, 1, opponnent_goal));
    }

    return has_goal_direction;
}

bool can_kick_to_goal(const board_status &current_board, player_status player, std::vector<std::pair<int, int>> &opponnent_goal)
{
    bool can_kick = false;

    for (int goal_row = 0; goal_row < 3; goal_row++)
    {
        // esta en diagonal al arco
        can_kick |= (is_in_same_rect(player.i, player.j, goal_row, -1, opponnent_goal)
                     && !opponents_blocking_goal(current_board, player, goal_row, -1, opponnent_goal));

        // esta derecho al arco
        can_kick |= (is_in_same_rect(player.i, player.j, goal_row, 0, opponnent_goal)
                     && !opponents_blocking_goal(current_board, player, goal_row, 0, opponnent_goal));

        // esta en diagonal al arco
        can_kick |= (is_in_same_rect(player.i, player.j, goal_row, 1, opponnent_goal)
                     && !opponents_blocking_goal(current_board, player, goal_row, 1, opponnent_goal));
    }

    return can_kick;
}

bool in_same_position(const player_status &p1, int dir1, const player_status &p2, int dir2)
{
    std::pair<int, int> new_p1_pos = std::make_pair(p1.i + moves[dir1].first, p1.j + moves[dir1].second);
    std::pair<int, int> new_p2_pos = std::make_pair(p2.i + moves[dir2].first, p2.j + moves[dir2].second);

    return new_p1_pos == new_p2_pos;
}

bool position_is_in_board(int i, int j, int rows, int columns)
{
    return 0 <= i && i < rows && 0 <= j && j < columns;
}

bool is_valid_kick(const player_status &player, int dir, int steps, int rows, int columns, std::vector<std::pair<int, int>> opponnent_goal)
{
    bool isValid = false;

    const int i = player.i + (moves[dir].first * 2) * steps;
    const int j = player.j + (moves[dir].second * 2) * steps;

    if (position_is_in_board(i, j, rows, columns))
    {
        isValid = true;
    }

    for (const auto &goal : opponnent_goal)
    {
        if (std::make_pair(i, j) == goal || std::make_pair(i - moves[dir].first, j - moves[dir].second) == goal)
        {
            isValid = true;
        }
    }

    return isValid;
}

int calculate_max_steps(const player_status &player, int dir, int rows, int columns, std::vector<std::pair<int, int>> opponnent_goal)
{
    const int middle_row = rows / 2;
    int steps = 0;

    for (int k = 0; k <= middle_row; k++)
    {
        if (is_valid_kick(player, dir, k, rows, columns, opponnent_goal))
        {
            steps = k;
        }
    }

    return steps;
}

bool inside_board(const player_status &player, int dir, std::vector<std::pair<int, int>> opponnent_goal, int rows, int columns)
{
    const int i = player.i + moves[dir].first;
    const int j = player.j + moves[dir].second;

    if (player.in_posetion)
    {
        for (const auto &g : opponnent_goal)
        {
            if (g.first == i && g.second == j) return true;
        }
    }

    return i >= 0 && j >= 0 && i < rows && j < columns;
}

bool in_different_positions(const std::vector<player_status> &team, std::vector<int> &player_moves)
{
    return !(in_same_position(team[0], player_moves[0], team[1], player_moves[1])
            || in_same_position(team[0], player_moves[0], team[2], player_moves[2])
            || in_same_position(team[1], player_moves[1], team[2], player_moves[2]));
}

double distance_ball_to_opp_goal(ball_status &ball, std::vector<std::pair<int, int>> opponnent_goal)
{
    const double di1 = distance(ball.i, ball.j, opponnent_goal[0].first, opponnent_goal[0].second);
    const double di2 = distance(ball.i, ball.j, opponnent_goal[1].first, opponnent_goal[1].second);
    const double di3 = distance(ball.i, ball.j, opponnent_goal[2].first, opponnent_goal[2].second);

    return std::min(di1, std::min(di2, di3));
}

bool scored(const board_status &board, std::vector<std::pair<int, int>> opponnent_goal)
{
    for (const auto &goal : opponnent_goal)
    {
        if (get_ball_position(board) == goal)
        {
            return true;
        }
    }
    return false;
}
