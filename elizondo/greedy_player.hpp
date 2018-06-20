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
                    std::vector<player_status> team = current_board.team;
                    
                    if (team[0].in_posetion && valid_positions(team, 8, j, k)) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 0 patee y los otros se muevan en direccion j y k
                            current_rank = evaluate_board(current_board, i, j, k, 0, steps);
                            //evaluar que los jugadores se muevan en direccion i, j y k

                            max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k, 0, steps);
                        }
                    }
                    
                    if (team[1].in_posetion && valid_positions(team, i, 8, k)) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 1 patee y los otros se muevan en direccion i y k
                            current_rank = evaluate_board(current_board, i, j, k, 1, steps);
                            //evaluar que los jugadores se muevan en direccion i, j y k
                        
                            max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k, 1, steps);
                        }
                    }
                    
                    if (team[2].in_posetion && valid_positions(team, i, j, 8)) {
                        for (int steps = 1; steps <= max_steps; ++steps) {
                            //evaluar que jugador 2 patee y los otros se muevan en direccion i y j
                            current_rank = evaluate_board(current_board, i, j, k, 2, steps);
                        
                            max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k, 2, steps);
                        }
                    }
                    
                    if (valid_positions(team, i, j, k)) {
                        current_rank = evaluate_board(current_board, i, j, k);
                        
                        max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k);
                    }
                }
            }
        }
    }

    void finish(std::string result) { }

private:

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

    void update_move(int id, player_move& current_move, int index, int steps) {
        current_move.player_id = id;
        current_move.move_type = steps == 0 ? MOVIMIENTO : PASE;
        current_move.dir = moves[index].number;
        current_move.steps = steps;
    }

    bool valid_positions(const std::vector<player_status>& team, int i, int j, int k) {
        return valid_position(team[0], i) && valid_position(team[1], j) && valid_position(team[2], k) && in_different_positions(team, i, j, k);
    }

    bool valid_position(const player_status &player, int index) {
        return (player.i + moves[index].i >= 0) && (player.j + moves[index].j >= 0) && (player.i + moves[index].i < rows) && (player.j + moves[index].j < columns);
    }

    bool in_different_positions(const std::vector<player_status>& team, int i, int j, int k) {
        return !(in_same_position(team[0], i, team[1], j) || in_same_position(team[0], i, team[2], k) || in_same_position(team[1], j, team[2], k));
    }

    bool in_same_position(const player_status& p1, int idx1, const player_status& p2, int idx2) {
        return (p1.i + moves[idx1].i == p2.i + moves[idx2].i) && (p1.j + moves[idx1].j == p2.j + moves[idx2].j);
    }

  double distance(int ball_i, int ball_j, int player_i, int player_j) {
        double x = static_cast<double>(ball_i) - static_cast<double>(player_i);
        double y = static_cast<double>(ball_j) - static_cast<double>(player_j);
      return sqrt(pow(x, 2) + pow(y, 2));
  }

  int evaluate_board(const board_status &board, int i, int j, int k, int player_with_ball = 0, int steps = 0) {   //Si steps no es 0, entonces miro en player_with_ball cual tiene la pelota
        player_status player_0 = board.team[0];
        player_status player_1 = board.team[1];
        player_status player_2 = board.team[2];
        int board_ranking = 0;
        double MAX_DIST = distance(0, 0, this->columns, this->rows); //
        // por ahora solovemos el puntaje cuando nuestro equpio no tiene la pelota
        if (steps == 0) {
            // hace el movimiento, o sea pone a los jugadores en donde deberían estar
            player_0.i += moves[i].i;
            player_0.j += moves[i].j;
            player_1.i += moves[j].i;
            player_1.j += moves[j].j;
            player_2.i += moves[k].i;
            player_2.j += moves[k].j;

            // cuando no tenemos la pleota puntuamos bien tenerla cerca (despues se verá)
            double distance_to_ball_p0 = MAX_DIST - distance(board.ball.i, board.ball.j, player_0.i, player_0.j);
            double distance_to_ball_p1 = MAX_DIST - distance(board.ball.i, board.ball.j, player_1.i, player_1.j);
            double distance_to_ball_p2 = MAX_DIST - distance(board.ball.i, board.ball.j, player_2.i, player_2.j);
            int ball_distance_ranking = static_cast<int>(distance_to_ball_p0 + distance_to_ball_p1 + distance_to_ball_p2);
            board_ranking += ball_distance_ranking;
        }

        return board_ranking;
    }
};
