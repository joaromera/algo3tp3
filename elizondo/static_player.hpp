#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"
#include "player.h"

std::random_device rd;
std::mt19937 generator(rd());

class static_player
{
public:
    static_player() = default;

    static_player(
        int pColumns,
        int pRows,
        int pSteps,
        std::string pSide,
        const std::vector<Player> &,
        const std::vector<Player> &)
        : columns(pColumns), rows(pRows), steps(pSteps), side(pSide)
    {
    }

    void starting_positions(std::vector<player_status> &positions)
    {
        int column = columns - 1;

        if (side == IZQUIERDA)
        {
            column = 0;
        }

        for (int i = 0; i < 3; ++i)
        {
            positions.emplace_back(i, i, column, false);
        }
    }

    void make_move(const board_status &current_board, std::vector<player_move> &made_moves)
    {
        made_moves.clear();

        for (const auto &p : current_board.team)
        {
            made_moves.emplace_back(p.id, MOVIMIENTO, 0, 0);
        }
    }

    void finish(std::string) {}

private:

    int columns;
    int rows;
    int steps;
    std::string team;
    std::string side;
};