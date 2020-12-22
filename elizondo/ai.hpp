#pragma once

#include <string>
#include <vector>

#include "board_status.hpp"

class AI
{
public:
    virtual ~AI() = default;

    virtual void starting_positions(std::vector<player_status> &positions) const = 0;

    virtual void make_move(const board_status &current_board, std::vector<player_move> &made_moves) = 0;

    void finish(const std::string &) {}
};
