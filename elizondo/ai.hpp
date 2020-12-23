#pragma once

#include <string>
#include <vector>

#include "board_status.hpp"
#include "constants.hpp"

class AI
{
public:
    virtual ~AI() = default;

    virtual std::vector<player_status> starting_positions(int columns, const std::string &side) const
    {
        int column = columns - 1;

        if (side == IZQUIERDA)
        {
            column = 0;
        }

        std::vector<player_status> positions;

        for (int i = 0; i < 3; ++i)
        {
            positions.emplace_back(i, i, column, false);
        }

        return positions;
    }

    virtual void make_move(const board_status &current_board, std::vector<player_move> &made_moves) = 0;

    void finish(const std::string &) {}
};
