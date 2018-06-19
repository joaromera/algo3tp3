#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"

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
        int max_steps = columns/2;
        for (int i = 0; i < moves.size(); i++) {
            for (int j = 0; j < moves.size(); j++) {
                for (int k = 0; k < moves.size(); k++) {
                    int current_rank = 0;
                    //aplicar refactor a la condicion
                    if (current_board.team[0].in_posetion && valid_position(current_board.team[1], moves[j]) && valid_position(current_board.team[2], moves[k])) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 0 patee y los otros se muevan en direccion j y k
                            current_rank = evaluate_board(current_board, moves[8], moves[j], moves[k], moves[i], steps);
                            //evaluar que los jugadores se muevan en direccion i, j y k
                            if (current_rank > max_rank) {
                                update_move(0, steps, made_moves[0]);
                                update_move(1, moves[j], made_moves[1]);
                                update_move(2, moves[k], made_moves[2]);

                                max_rank = current_rank;
                            }
                        }
                    }
                    //aplicar refactor a la condicion
                    if (current_board.team[1].in_posetion && valid_position(current_board.team[0], moves[i]) && valid_position(current_board.team[2], moves[k])) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 1 patee y los otros se muevan en direccion i y k
                            current_rank = evaluate_board(current_board, moves[i], moves[8], moves[k], moves[j], steps);
                            //evaluar que los jugadores se muevan en direccion i, j y k
                            if (current_rank > max_rank) {
                                update_move(0, moves[i], made_moves[0]);
                                update_move(1, steps, made_moves[1]);
                                update_move(2, moves[k], made_moves[2]);

                                max_rank = current_rank;
                            }
                        }
                    }
                    //aplicar refactor a la condicion
                    if (current_board.team[2].in_posetion && valid_position(current_board.team[1], moves[j]) && valid_position(current_board.team[1], moves[j])) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 2 patee y los otros se muevan en direccion i y j
                            current_rank = evaluate_board(current_board, moves[i], moves[j], moves[8], moves[k], steps);
                            //evaluar que los jugadores se muevan en direccion i, j y k
                            if (current_rank > max_rank) {
                                update_move(0, moves[i], made_moves[0]);
                                update_move(1, moves[j], made_moves[1]);
                                update_move(2, steps, made_moves[2]);

                                max_rank = current_rank;
                            }
                        }
                    }
                    //aplicar refactor a la condicion
                    if (valid_position(current_board.team[0], moves[i]) && valid_position(current_board.team[1], moves[j]) && valid_position(current_board.team[2], moves[k])) {
                        current_rank = evaluate_board(current_board, moves[i], moves[j], moves[k]);
                        //evaluar que los jugadores se muevan en direccion i, j y k
                        if (current_rank > max_rank) {
                            update_move(0, moves[i], made_moves[0]);
                            update_move(1, moves[j], made_moves[1]);
                            update_move(2, moves[k], made_moves[2]);

                            max_rank = current_rank;
                        }
                    }
                }
            }
        }

        //borrar

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

private:

    void update_move(int id, const move &move, player_move& current_move) {
        current_move.player_id = id;
        current_move.move_type = MOVIMIENTO;
        current_move.dir = move.number;
    }

    void update_move(int id, int steps, player_move& current_move) {
        current_move.player_id = id;
        current_move.move_type = PASE;
        current_move.dir = 8;
        current_move.steps = steps;
    }

    bool valid_position(const player_status &player, const move &move) {
        return (player.i + move.i < rows) && (player.j + move.j < columns);
    }

    int evaluate_board(const board_status &board, const move &move_i, const move &move_j, const move &move_k) {
        return 0;
    }

    int evaluate_board(const board_status &board, const move &move_i, const move &move_j, const move &move_k, const move &move_ball,
                       const int steps) {
        return 0;
    }
};
