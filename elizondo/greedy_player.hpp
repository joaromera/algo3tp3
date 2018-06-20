#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"

random_device rd;
mt19937 generator(rd());

class greedy_player {
    
    int columns, rows, steps;
    string team, side;
    const vector<player>* players;
    const vector<player>* oponents;

public:

    greedy_player() = default;

    greedy_player(
        int columns,
        int rows,
        int steps,
        string side,
        const vector<player>& players,
        const vector<player>& oponent_players
    ) {
        this->columns = columns;
        this->rows = rows;
        this->steps = steps;
        this->side = side;
        this->team = team;

        this->players = &players;
        this->oponents = &oponent_players;
        
        LogicalBoard* logical_board = new LogicalBoard(
            columns,
            rows,
            players,
            oponent_players
        );
    }

    void starting_positions(vector<player_status>& positions) {
        int column = this->columns - 1;
        if (this->side == IZQUIERDA) {
            column = 0;
        }

        for (int i = 0; i < 3; i++) {
            positions.emplace_back(i, i, column, false);
        }
    }

    // Aca se usa la función punteadora, greedy, genetica, etc
    void make_move(const board_status& current_board, vector<player_move>& made_moves) {
        int max_rank = 0;
        for (int i = 0; i < moves.size(); i++) {
            for (int j = 0; j < moves.size(); j++) {
                for (int k = 0; k < moves.size(); k++) {
                    for(int jugador = 0; jugador < 3; ++jugador) {
                        vector<int> player_moves{ i, j, k };
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

    void finish(string result) { }

private:

    int calculate_max_steps(const player_status &player, int dir, int rows, int columns) {
        int half_board = columns/2;
        int steps = 0;
        int i = player.i;
        int j = player.j;
        while (i < rows && j < columns) {
            i += _moves[dir].i;
            j += _moves[dir].j;
            ++steps;
        }

        return steps < half_board ? steps : half_board;
    }

    int calculate_max_board_for_player_passes(const board_status& current_board, int max_steps, int max_rank, vector<player_move>& made_moves, int i, int j, int k, int player_with_ball) {
        vector<int> player_moves{ i, j, k };
        player_moves[player_with_ball] = 8;

        if (current_board.team[player_with_ball].in_posetion && valid_positions(current_board.team, player_moves[0], player_moves[1], player_moves[2])) {
            for (int steps = 1; steps <= max_steps; ++steps) {
                int current_rank = evaluate_board(current_board, i, j, k, player_with_ball, steps);
            
                max_rank = update_moves(max_rank, current_rank, made_moves, i, j, k, player_with_ball, steps);
            }
        }

        return max_rank;
    }

    int update_moves(int max_rank, int current_rank, vector<player_move>& made_moves, int i, int j, int k, int player_with_ball = 0, int steps = 0) {
        vector<int> steps_players(3, 0);
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
        current_move.dir = _moves[dir].number;
        current_move.steps = steps;
    }

    bool valid_positions(const vector<player_status>& team, int i, int j, int k) {
        return valid_position(team[0], i) && valid_position(team[1], j) && valid_position(team[2], k) && in_different_positions(team, i, j, k);
    }

    bool valid_position(const player_status &player, int dir) {
        return (player.i + _moves[dir].i >= 0) && (player.j + _moves[dir].j >= 0) && (player.i + _moves[dir].i < rows) && (player.j + _moves[dir].j < columns);
    }

    bool in_different_positions(const vector<player_status>& team, int i, int j, int k) {
        return !(in_same_position(team[0], i, team[1], j) || in_same_position(team[0], i, team[2], k) || in_same_position(team[1], j, team[2], k));
    }

    bool in_same_position(const player_status& p1, int dir1, const player_status& p2, int dir2) {
        return (p1.i + _moves[dir1].i == p2.i + _moves[dir2].j) && (p1.j + _moves[dir1].j == p2.j + _moves[dir2].j);
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
            player_0.i += _moves[i].i;
            player_0.j += _moves[i].j;
            player_1.i += _moves[j].i;
            player_1.j += _moves[j].j;
            player_2.i += _moves[k].i;
            player_2.j += _moves[k].j;

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
