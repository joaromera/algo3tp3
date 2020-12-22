#pragma once

#include <initializer_list>
#include <string>
#include <vector>

struct move
{
    int number;
    int i;
    int j;

    move(std::initializer_list<int> params)
    {
        auto it = params.begin();
        number = *(it++);
        i = *(it++);
        j = *(it++);
    }
};

struct player_status
{
    int id;
    int i;
    int j;
    bool in_posetion = false;

    player_status() {}
    player_status(int id) : id(id) {}
    player_status(int id, int i, int j, bool in_posetion) : id(id), i(i), j(j), in_posetion(in_posetion) {}
};

struct ball_status
{
    int i;
    int j;
    int dir;
    int steps;
    bool is_free = false;
};

struct player_move
{
    int player_id;
    std::string move_type;
    int dir;
    int steps;

    player_move() = default;
    player_move(int pId, const std::string& pMove, int pDir, int pSteps) : player_id(pId), move_type(pMove), dir(pDir), steps(pSteps) {}
};

struct board_status
{
    std::vector<player_status> team;
    std::vector<player_status> oponent_team;
    ball_status ball;

    void clear()
    {
        team.clear();
        oponent_team.clear();
        ball.is_free = false;
    }
};

bool is_neighbor(int &i, int &j, const std::vector<std::tuple<int, int>> &goal)
{
    for (const auto &c : goal)
    {
        if (abs(i - std::get<0>(c)) < 2 &&
            abs(j - std::get<1>(c)) < 2 &&
            i != std::get<0>(c) &&
            j != std::get<1>(c))
        {
            return true;
        }
    }
    return false;
}
