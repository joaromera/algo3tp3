#include <string>
#include <random>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"

random_device rd;
mt19937 generator(rd());

using namespace std;

class greedy_player {
    
    int columns, rows, steps;
    string team, side;
    vector<pair <int, int> > own_goal;
    vector<pair <int, int> > opponnent_goal;
    vector<player> players;
    vector<player> opponnents;

public:

    greedy_player() = default;

    greedy_player(
        int columns,
        int rows,
        int steps,
        string side,
        const vector<player>& players,
        const vector<player>& opponnent_players
    ) {
        this->columns = columns;
        this->rows = rows;
        this->steps = steps;
        this->side = side;
        this->team = team;
        this->players = players;
        this->opponnents = opponnent_players;
        this->get_goal_positions();

    }

    void get_goal_positions() {
        int mid_row = this->rows / 2;
        int column_own_goal = this->columns;
        int column_opponnent_goal = -1;
        if (this->side == IZQUIERDA) {
            column_own_goal = -1;
            column_opponnent_goal = this->columns;
        }

        for (int i = -1; i < 2; i++) {
            own_goal.push_back(make_pair(mid_row + i, column_own_goal));
            opponnent_goal.push_back(make_pair(mid_row + i, column_opponnent_goal));
        }

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
        made_moves.clear();
        player_move p_move;
        made_moves.push_back(p_move);
        made_moves.push_back(p_move);
        made_moves.push_back(p_move);
        
        double max_rank = -9999999;
        double current_rank = -9999999;

        for (int i = 0; i < moves.size(); i++) {
            if (inside_board(current_board.team[0], i)) {        
                for (int j = 0; j < moves.size(); j++) {
                    if (inside_board(current_board.team[1], j)) {
                        for (int k = 0; k < moves.size(); k++) {
                            if (inside_board(current_board.team[2], k)) {
                                if (in_different_positions(current_board.team, i, j, k)) {
                                    for (int jugador = 0; jugador < 3; jugador++) {
                                        vector<int> player_moves { i, j, k };
                                        if (current_board.team[jugador].in_posetion && player_moves[jugador] != 0 && kicking_different_positions(current_board.team, i, j, k, jugador)) {
                                            int max_steps = calculate_max_steps(current_board.team[jugador], player_moves[jugador]);
                                            for (int steps = 1; steps <= max_steps; steps++) {
                                                current_rank = evaluate_board(current_board, i, j, k, jugador, steps);
                                                if (current_rank > max_rank) {
                                                    max_rank = current_rank;
                                                    update_moves(current_board, made_moves, i, j, k, jugador, steps);
                                                }
                                            }
                                        }
                                    }
                                    current_rank = evaluate_board(current_board, i, j, k);
                                    if (current_rank > max_rank) {
                                        max_rank = current_rank;
                                        update_moves(current_board, made_moves, i, j, k);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void finish(string result) { }

private:

    int calculate_max_steps(const player_status &player, int dir) {
        //la pelota no puede avanzar mas que la mitad de filas de la cancha
        int middle_row = rows/2;
        int steps = 0;
        int i = player.i;
        int j = player.j;

        i += moves[dir].first * 2;
        j += moves[dir].second * 2;
        
        bool is_in_range = true;
        //mientras este en la cancha o algun arco, itero
        while (is_in_range) {
            //me fijo que si avanzo la pelota 1 vez siga en la cancha
            if (position_is_in_board(i, j)) {
                //actualizo la posicion y sumo uno a la cantidad maxima de steps
                steps++;
                i += moves[dir].first * 2;
                j += moves[dir].second * 2;
            } else if (ball_is_in_any_goal(i, j, dir)) {
                //si la pelota llego al arco con el proximo step, entonces
                //sumo uno a la cantidad maxima de steps y salgo del ciclo
                steps++;
                is_in_range = false;
            } else {
                //sino la pelota salio del ciclo
                is_in_range = false;
            }
        }

        //si la cantidad maxima de steps es mayor a la mitad de filas
        //retorno la mitad de las filas como maximo step
        return steps < middle_row ? steps : middle_row;
    }

    bool position_is_in_board(int i, int j) {
        for (int index = 0; index < 3; index++) {
            // verifico si la pelota llega a la posicion "index" de algun arco con 1 pase o medio
            // if (pos_half_pass == this->opponnent_goal[index] || pos_full_pass == this->opponnent_goal[index]) return true;
            if (i == this->opponnent_goal[index].first && j == this->opponnent_goal[index].second) return true;
        }
        return  0 <= i && i < rows && 0 <= j && j < columns;
    }

    bool ball_is_in_any_goal(int i, int j, int dir) {
        //hay 2 casos, en el que la pelota con un pase mas entre en el arco
        //o que con medio pase más entre (por que estaba a 1 casillero de distancia)
        // int i_half_pass = i - moves[dir].first;
        // int j_half_pass = j - moves[dir].second;
        // pair <int, int> pos_half_pass = make_pair(i_half_pass, j_half_pass);

        pair <int, int> pos_full_pass = make_pair(i, j);

        //como los arcos tienen 3 posiciones, itero de [0, 3)
        for (int index = 0; index < 3; index++) {
            // verifico si la pelota llega a la posicion "index" de algun arco con 1 pase o medio
            // if (pos_half_pass == this->opponnent_goal[index] || pos_full_pass == this->opponnent_goal[index]) return true;
            if (i == this->opponnent_goal[index].first && j == this->opponnent_goal[index].second) return true;
        }

        //si no cayo en el return del ciclo, entonces la pelota no entra al arco
        return false;
    }

    void update_moves(const board_status& current_board, vector<player_move>& made_moves, int i, int j, int k, int player_with_ball = 0, int steps = 0) {
        //si nadie patea todos tienen step 0
        vector<int> steps_players(3, 0);
        //si algun jugador pateo, actualizo su step
        steps_players[player_with_ball] = steps;

        update_move(current_board.team[0].id, made_moves[0], i, steps_players[0]);
        update_move(current_board.team[1].id, made_moves[1], j, steps_players[1]);
        update_move(current_board.team[2].id, made_moves[2], k, steps_players[2]);        
    }

    void update_move(int id, player_move& current_move, int dir, int steps) {
        current_move.player_id = id;
        current_move.move_type = steps == 0 ? MOVIMIENTO : PASE;
        current_move.dir = dir;
        current_move.steps = steps;
    }

    bool valid_positions(const vector<player_status>& team, int i, int j, int k) {
        //verifico que todos los jugaores esten dentro de la cancha y que ninguno comparta posicion
        return inside_board(team[0], i) && inside_board(team[1], j) && inside_board(team[2], k) && in_different_positions(team, i, j, k);
    }

    bool inside_board(const player_status &player, int dir) {
        int i = player.i + moves[dir].first;
        int j = player.j + moves[dir].second;

        if (player.in_posetion) {
            for (auto g : this->opponnent_goal) {
                if (g.first == i && g.second == j) return true;
            }
        }

        return i >= 0 && j >= 0 && i < rows && j < columns;
    }

    bool in_different_positions(const vector<player_status>& team, int i, int j, int k) {
        return !(in_same_position(team[0], i, team[1], j) || in_same_position(team[0], i, team[2], k) || in_same_position(team[1], j, team[2], k));
    }

    bool kicking_different_positions(const vector<player_status>& team, int i, int j, int k, int jugador) {
        if (team[jugador].in_posetion) {
            switch(jugador) {
                case 0: i = 0; break;
                case 1: j = 0; break;
                case 2: k = 0; break;
                }        
        }
        return in_different_positions(team, i, j, k);
    }

    bool in_same_position(const player_status& p1, int dir1, const player_status& p2, int dir2) {
        //le sumo a la posicion del jugador el movimiento que realiza
        pair<int, int> new_p1_pos = make_pair(p1.i + moves[dir1].first, p1.j + moves[dir1].second);
        pair<int, int> new_p2_pos = make_pair(p2.i + moves[dir2].first, p2.j + moves[dir2].second);

        return new_p1_pos == new_p2_pos;
    }

    double evaluate_board(const board_status& current_board, int i, int j, int k, int jugador = 0, int steps = 0) {
        
        LogicalBoard* logical_board = new LogicalBoard(
            this->columns,
            this->rows,
            this->players,
            this->opponnents,
            current_board
        );

        vector<player_move> moves_A = {
            {current_board.team[0].id, "MOVIMIENTO", i},
            {current_board.team[1].id, "MOVIMIENTO", j},
            {current_board.team[2].id, "MOVIMIENTO", k}
        };

        if (steps > 0) {
            for (auto pm : moves_A) {
                if (pm.player_id == jugador) {
                    pm.move_type = "PASE";
                    pm.steps = steps;
                }
            }
        }

        vector<player_move> moves_B = {
            {current_board.oponent_team[0].id, "MOVIMIENTO", 0},
            {current_board.oponent_team[1].id, "MOVIMIENTO", 0},
            {current_board.oponent_team[2].id, "MOVIMIENTO", 0}
        };

        logical_board->makeMove(moves_A, moves_B);
        
        board_status updated_board = logical_board->getNewState();        
        
        double result = 0;

        // Mientras más cerca esté la pelota del arco contrario, mejor
        //result -= distance_ball_opponnent_goal(aftermove);

        if (who_has_the_ball(updated_board) == "GREEDY") {
            // Queremos que se acerquen al arco contrario y que se alejen de los oponentes
            for (auto p : updated_board.team) {
                result -= distance_player_opponnent_goal(p);
                result += distance_player_closest_opponnent(updated_board, p);
                if (p.in_posetion) {
                    if (can_kick_to_goal(updated_board, p)) {
                        // hay que pensar bien el valor
                        //result += 1;
                    }
                }
            }
        } else if (who_has_the_ball(updated_board) == "OPPONNENT") {
            // Queremos que se acerquen a los contrarios
            for (auto p : updated_board.team) {
                result -= distance_player_closest_opponnent(updated_board, p);
            }
        } else {
            // Queremos que se acerquen a la pelota porque está libre
            for (auto p : updated_board.team) {
                result -= distance_player_ball(updated_board, p);
            }   
        }

        if (ball_is_in_any_goal(updated_board.ball.i, updated_board.ball.j, 0)) {
            result += 1000;
        }

        return result;
    }

    bool can_kick_to_goal(const board_status& current_board, player_status player) {
        // me fijo si esta en linea recta al arco
        bool is_goal_row = opponnent_goal[0].first == player.i || opponnent_goal[1].first == player.i || opponnent_goal[2].first == player.i;

        bool can_kick = false;
        for (int i = 0; i < 3; i++) {
            // el jugador esta arriba del arco
            if (player.i > opponnent_goal[i].first) {
                // esta en diagonal al arco (pendiente negativa)
                if (is_in_same_rect(player, i, -1)) {
                    can_kick = !opponents_blocking_goal(current_board, player, i, -1);
                }
            } else {
                // esta en diagonal al arco (pendiente positiva)
                if (is_in_same_rect(player, i, 1)) {
                    can_kick = !opponents_blocking_goal(current_board, player, i, 1);
                }
            }
        }

        return is_goal_row || can_kick;
    }

    bool is_in_same_rect(player_status player, int goal_row, int a) {
        // y = a*x => y - a*x = 0
        // a puede ser 1 o -1
        return opponnent_goal[goal_row].second + a*opponnent_goal[goal_row].first == player.j + a*player.i;
    }

    bool closer_to_opponent_goal(int player_i, int opp_player_i) {
        // dice si el jugador del equipo greedy esta entre el oponente y su arco
        return abs(opponnent_goal[0].first - player_i) < abs(opponnent_goal[0].first - opp_player_i); 
    }

    bool opponents_blocking_goal(const board_status& current_board, player_status player, int goal_row, int a) {
        bool is_in_between = false;
        for (auto opp_p : current_board.oponent_team) {
            is_in_between = is_in_between || is_in_same_rect(opp_p, goal_row, a) && !closer_to_opponent_goal(player.i, opp_p.i);
        }
        return is_in_between;
    }

    double distance(int from_i, int from_j, int to_i, int to_j) {
        double x = static_cast<double>(from_i) - static_cast<double>(to_i);
        double y = static_cast<double>(from_j) - static_cast<double>(to_j);
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    // Queremos que sea bajo
    double distance_ball_opponnent_goal(const board_status& current_board) {
        int ball_i;
        int ball_j;

        if (current_board.ball.is_free) {
            ball_i = current_board.ball.i;
            ball_j = current_board.ball.j;
        } else {
            for (auto p : current_board.team) {
                if (p.in_posetion) {
                    ball_i = p.i;
                    ball_j = p.j;
                }
            }
            for (auto p : current_board.oponent_team) {
                if (p.in_posetion) {
                    ball_i = p.i;
                    ball_j = p.j;
                }
            }
        }

        double di1 = distance(ball_i, ball_j, this->opponnent_goal[0].first, this->opponnent_goal[0].second);
        double di2 = distance(ball_i, ball_j, this->opponnent_goal[1].first, this->opponnent_goal[1].second);
        double di3 = distance(ball_i, ball_j, this->opponnent_goal[2].first, this->opponnent_goal[2].second);
    
        return min(di1,min(di2,di3));
    }

    // Si tenemos la pelota queremos que sea grande, si no la tenemos queremos que sea baja
    double distance_player_closest_opponnent(const board_status& current_board, player_status& player) {
        double di1 = distance(player.i, player.j, current_board.oponent_team[0].i, current_board.oponent_team[0].j);
        double di2 = distance(player.i, player.j, current_board.oponent_team[1].i, current_board.oponent_team[1].j);
        double di3 = distance(player.i, player.j, current_board.oponent_team[2].i, current_board.oponent_team[2].j);
    
        return min(di1,min(di2,di3));
    }

    // Con la pelota queremos que sea bajo
    double distance_player_opponnent_goal(player_status& player) {
        double di1 = distance(player.i, player.j, this->opponnent_goal[0].first, this->opponnent_goal[0].second);
        double di2 = distance(player.i, player.j, this->opponnent_goal[1].first, this->opponnent_goal[1].second);
        double di3 = distance(player.i, player.j, this->opponnent_goal[2].first, this->opponnent_goal[2].second);
    
        return min(di1,min(di2,di3));
    }

    // Si no tenemos la pelota queremos que sea chico
    int distance_player_ball(const board_status& current_board, player_status& player) {
        int di1 = distance(player.i, player.j, current_board.ball.i, current_board.ball.j);
        int di2 = distance(player.i, player.j, current_board.ball.i, current_board.ball.j);
        int di3 = distance(player.i, player.j, current_board.ball.i, current_board.ball.j);
    
        return min(di1,min(di2,di3));
    }

    string who_has_the_ball(const board_status& current_board) {
        for (auto p : current_board.team) {
            if (p.in_posetion) {
                return "GREEDY";
            }
        }
        for (auto p : current_board.oponent_team) {
            if (p.in_posetion) {
                return "OPPONNENT";
            }
        }
        return "FREE";
    }

};
