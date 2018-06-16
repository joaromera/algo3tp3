#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"

std::random_device rd;
std::mt19937 generator(rd());

class static_player {
    
    int columns, rows, steps;
    std::string team, side;

public:

    static_player() = default;

    static_player(
        int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<player>& players, // Este no lo usa para nada
        const std::vector<player>& oponent_players // Este no lo usa para nada
    ) {
        this->columns = columns;
        this->rows = rows;
        this->steps = steps;
        this->side = side;
        this->team = team;
    }

    void starting_positions(std::vector<player_status>& positions) {
        int column = this->columns - 1;
        if (this->side == IZQUIERDA) {
            column = 0;
        }

        for (int i = 0; i < 3; i++) {
            positions.emplace_back(i, i, column, false);
        }
    }

    void make_move(const board_status& current_board, std::vector<player_move>& made_moves) {
        made_moves.clear();
        player_move new_move;

        for (auto& p : current_board.team) {
            new_move.player_id = p.id;
            new_move.move_type = MOVIMIENTO;
            new_move.dir = 0;
            made_moves.push_back(new_move);
        }
    }

    void finish(std::string result) { }
};