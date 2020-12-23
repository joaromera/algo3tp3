#include <string>
#include <vector>

#include "ai.hpp"
#include "board_status.hpp"
#include "constants.hpp"
#include "player.h"

class static_player : public AI
{
public:
    static_player() = default;

    static_player(
        int pColumns,
        int,
        int,
        std::string pSide,
        const std::vector<Player> &,
        const std::vector<Player> &)
        : AI(pColumns, pSide)
    {
    }

    void make_move(const board_status &current_board, std::vector<player_move> &made_moves) override
    {
        made_moves.clear();

        for (const auto &p : current_board.team)
        {
            made_moves.emplace_back(p.id, MOVIMIENTO, 0, 0);
        }
    }

    void finish(std::string) {}

private:

    std::string team;
};
