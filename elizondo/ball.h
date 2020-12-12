#pragma once

#include <map>
#include <random>
#include <string>
#include <vector>

#include "constants.hpp"

class Ball
{
public:
    int i = 0;
    int j = 0;
    std::tuple<int, int> movement { -1, -1 };

    Ball()
    {
    }

    Ball(const int pI, const int pJ)
    : i(pI)
    , j(pJ)
    {
    }

    ~Ball() {}

    void setMovement(const int dir, const int steps)
    {
        movement = std::make_tuple(dir, steps);
    }

    bool isStill()
    {
        return std::get<0>(movement) == -1 && std::get<1>(movement) == -1;
    }

    void move()
    {
        if (isStill())
        {
            return;
        }

        if (std::get<1>(movement) > 0)
        {
            const std::pair<int, int> move = moves[std::get<0>(movement)];
            i += 2 * move.first;
            j += 2 * move.second;
            std::get<1>(movement) = std::get<1>(movement) - 1;
        }
    }

    std::tuple<int, int> finalPosition()
    {
        const std::pair<int, int> move = moves[std::get<0>(movement)];
        const int steps = 2 * std::get<1>(movement);
        return { i + steps * move.first, j + steps * move.second };
    }

    std::vector<std::tuple<int, int>> trajectory()
    {
        const std::pair<int, int> move = moves[std::get<0>(movement)];
        const int steps = 2 * std::get<1>(movement);
        std::vector<std::tuple<int, int>> trajectory;
        for (int i = 0; i <= steps; ++i)
        {
            trajectory.push_back(std::make_tuple(i + i * move.first, j + i * move.second));
        }
        return trajectory;
    }

    void undoMove()
    {
        if (isStill())
        {
            return;
        }

        const std::pair<int, int> move = moves[std::get<0>(movement)];
        i -= 2 * move.first;
        j -= 2 * move.second;
        std::get<1>(movement) = std::get<1>(movement) + 1;
    }

    void step_back_one()
    {
        if (isStill())
        {
            return;
        }

        const std::pair<int, int> move = moves[std::get<0>(movement)];
        i -= move.first;
        j -= move.second;
    }
};
