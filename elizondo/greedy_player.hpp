#include <string>
#include <random>
#include <vector>
#include <map>

#include "constants.hpp"
#include "board_status.hpp"
#include "logical_board.cpp"

std::random_device rd;
std::mt19937 generator(rd());

class greedy_player {
    
    int columns, rows, steps;
    std::string team, side;

public:

    greedy_player() = default;

    greedy_player(
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

// Aca se usa la función punteadora, greedy, genetica, etc
    void make_move(const board_status& current_board, std::vector<player_move>& made_moves) {
        int max_rank = 0;
        for (int i = 0; i < moves.size(); i++) {
            for (int j = 0; j < moves.size(); j++) {
                for (int k = 0; k < moves.size(); k++) {
                    for(int jugador = 0; jugador < 3; ++jugador) {
                        std::vector<int> player_moves{ i, j, k };
                        int max_steps = calculate_max_steps(current_board.team[jugador], player_moves[jugador], rows, columns);
                        max_rank = calculate_max_board_for_player_passes(current_board, max_steps, max_rank, made_moves, i, j, k, jugador);
                    }
                    
                    if (valid_positions(current_board.team, i, j, k)) {
                        int current_rank = evaluate_board(current_board, i, j, k);
                        
                        max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k);
                    }
                }
            }
        }
    }

    void finish(std::string result) { }

private:

    int calculate_max_steps(const player_status &player, int dir, int rows, int columns) {
        int half_board = columns/2;
        int steps = 0;
        int i = player.i;
        int j = player.j;
        while (i < rows && j < columns) {
            i += moves[dir].first;
            j += moves[dir].second;
            ++steps;
        }

        return steps < half_board ? steps : half_board;
    }

    int calculate_max_board_for_player_passes(const board_status& current_board, int max_steps, int max_rank, std::vector<player_move>& made_moves, int i, int j, int k, int player_with_ball) {
        std::vector<int> player_moves{ i, j, k };
        player_moves[player_with_ball] = 8;

        if (current_board.team[player_with_ball].in_posetion && valid_positions(current_board.team, player_moves[0], player_moves[1], player_moves[2])) {
            for (int steps = 1; steps <= max_steps; ++steps) {
                int current_rank = evaluate_board(current_board, i, j, k, player_with_ball, steps);
            
                max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k, player_with_ball, steps);
            }
        }

        return max_rank;
    }

    int update_moves(int max_rank, int current_rank, std::vector<player_move>& made_moves, int i, int j, int k, int player_with_ball = 0, int steps = 0) {
        std::vector<int> steps_players(3, 0);
        steps_players[player_with_ball] = steps;

        if (current_rank > max_rank) {
            update_move(0, made_moves[0], i, steps_players[0]);
            update_move(1, made_moves[1], j, steps_players[1]);
            update_move(2, made_moves[2], k, steps_players[2]);

            max_rank = current_rank;
        }

        return max_rank;
    }

    void update_move(int id, player_move& current_move, int dir, int steps) {
        current_move.player_id = id;
        current_move.move_type = steps == 0 ? MOVIMIENTO : PASE;
        current_move.dir = dir;
        current_move.steps = steps;
    }

    bool valid_positions(const std::vector<player_status>& team, int i, int j, int k) {
        return valid_position(team[0], i) && valid_position(team[1], j) && valid_position(team[2], k) && in_different_positions(team, i, j, k);
    }

    bool valid_position(const player_status &player, int dir) {
        return (player.i + moves[dir].first >= 0) && (player.j + moves[dir].second >= 0) && (player.i + moves[dir].first < rows) && (player.j + moves[dir].second < columns);
    }

    bool in_different_positions(const std::vector<player_status>& team, int i, int j, int k) {
        return !(in_same_position(team[0], i, team[1], j) || in_same_position(team[0], i, team[2], k) || in_same_position(team[1], j, team[2], k));
    }

    bool in_same_position(const player_status& p1, int dir1, const player_status& p2, int dir2) {
        return (p1.i + moves[dir1].first == p2.i + moves[dir2].first) && (p1.j + moves[dir1].second == p2.j + moves[dir2].second);
    }

    int evaluate_board(const board_status &board, int i, int j, int k, int player_with_ball = 0, int steps = 0) {   //Si steps no es 0, entonces miro en player_with_ball cual tiene la pelota
        return 0;
    }
};
