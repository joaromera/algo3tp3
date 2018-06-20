#include <string>
#include <random>
#include <vector>
#include <map>
#include <iostream>

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
        //itero por todos los posibles mov del jugador 0
        for (int i = 0; i < moves.size(); i++) {
            //itero por todos los posibles mov del jugador 1
            for (int j = 0; j < moves.size(); j++) {
                //itero por todos los posibles mov del jugador 2
                for (int k = 0; k < moves.size(); k++) {
                    //por cada jugador evaluo la accion de patear
                    for(int jugador = 0; jugador < 3; ++jugador) {
                        std::vector<int> player_moves{ i, j, k };
                        //si el jugador que va a patear tiene direccion (0,0) no puede patear
                        if (player_moves[jugador] != 0) {
                            int max_steps = calculate_max_steps(current_board.team[jugador], player_moves[jugador], rows, columns);
                            max_rank = calculate_max_board_for_player_passes(current_board, max_steps, max_rank, made_moves, i, j, k, jugador);
                        }
                    }
                    //ahora verifico que los jugadores al realizar la combinacion
                    //de movimientos de esta iteracion sean posiciones validas
                    if (valid_positions(current_board.team, i, j, k)) {
                        int current_rank = evaluate_board(current_board, i, j, k);
                        
                        max_rank = update_rank_and_moves(max_rank, current_rank, made_moves, i, j, k);
                    }
                }
            }
        }
    }

    void finish(string result) { }

private:

    int calculate_max_steps(const player_status &player, int dir, int rows, int columns) {
        //la pelota no puede avanzar mas que la mitad de filas de la cancha
        int middle_row = rows/2;
        int steps = 0;
        int i = player.i;
        int j = player.j;

        bool is_in_range = true;
        //mientras este en la cancha o algun arco, itero
        while (is_in_range) {
            //me fijo que si avanzo la pelota 1 vez siga en la cancha
            if (position_is_in_board(i, j, dir, rows, columns)) {
                //actualizo la posicion y sumo uno a la cantidad maxima de steps
                i += moves[dir].first * 2;
                j += moves[dir].second * 2;
                ++steps;
            } else if (ball_is_in_any_goal(i, j, dir, rows, columns)) {
                //si la pelota llego al arco con el proximo step, entonces
                //sumo uno a la cantidad maxima de steps y salgo del ciclo
                ++steps;
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

    bool position_is_in_board(int i, int j, int dir, int rows, int columns) {
        //como la pelota se mueve de a 2 posiciones, me creo la prox posicion
        //y verifico que siga en la cancha
        int new_i = i + moves[dir].first * 2;
        int new_j = j + moves[dir].second * 2;

        return  0 <= new_i && new_i < rows && 0 <= new_j && new_j < columns;
    }

    bool ball_is_in_any_goal(int i, int j, int dir, int rows, int columns) {
        //hay 2 casos, en el que la pelota con un pase mas entre en el arco
        //o que con medio pase más entre (por que estaba a 1 casillero de distancia)
        int i_half_pass = i + moves[dir].first;
        int j_half_pass = j + moves[dir].second;
        std::pair <int, int> pos_half_pass = make_pair(i_half_pass, j_half_pass);

        int i_full_pass = i + moves[dir].first * 2;
        int j_full_pass = j + moves[dir].second * 2;
        std::pair <int, int> pos_full_pass = make_pair(i_full_pass, j_full_pass);

        //me genero los 2 arcos de la cancha
        int middle_row = rows/2;
        vector<std::pair <int, int> > goal_a = get_goal(middle_row, 0);
        vector<std::pair <int, int> > goal_b = get_goal(middle_row, columns);

        //como los arcos tienen 3 posiciones, itero de [0, 3)
        for (int index = 0; index < 3; ++index) {
            // verifico si la pelota llega a la posicion "index" de algun arco con 1 pase o medio
            if (pos_half_pass == goal_a[index] || pos_half_pass == goal_b[index]
                || pos_full_pass == goal_a[index] || pos_full_pass == goal_b[index]) return true;
        }

        //si no cayo en el return del ciclo, entonces la pelota no entra al arco
        return false;
    }

    vector<std::pair <int, int> > get_goal(int row, int col) {
        //obtengo el vector con las 3 posiciones del arco
        vector<std::pair <int, int> > goal;
        goal.push_back(make_pair(row-1, col));
        goal.push_back(make_pair(row, col));
        goal.push_back(make_pair(row+1, col));

        return goal;
    };

    int calculate_max_board_for_player_passes(const board_status& current_board, int max_steps, int max_rank, std::vector<player_move>& made_moves, int i, int j, int k, int player_with_ball) {
        //me guardo en un vector los movimientos de los jugadores
        std::vector<int> player_moves{ i, j, k };
        //este metodo se llama solamente cuando se patea
        //hago que el jugador que patee no se mueva
        player_moves[player_with_ball] = 0;

        //si el jugador que va a patear tiene la pelota y esta el equipo en posiciones validas
        if (current_board.team[player_with_ball].in_posetion && valid_positions(current_board.team, player_moves[0], player_moves[1], player_moves[2])) {
            //itero para todos los posibles valores de steps del jugador
            for (int steps = 1; steps <= max_steps; ++steps) {
                int current_rank = evaluate_board(current_board, i, j, k, player_with_ball, steps);
            
                max_rank = update_rank_and_moves(max_rank, current_rank, made_moves, i, j, k, player_with_ball, steps);
            }
        }

        return max_rank;
    }

    int update_rank_and_moves(int max_rank, int current_rank, std::vector<player_move>& made_moves, int i, int j, int k, int player_with_ball = 0, int steps = 0) {
        //si nadie patea todos tienen step 0
        std::vector<int> steps_players(3, 0);
        //si algun jugador pateo, actualizo su step
        steps_players[player_with_ball] = steps;

        //si consegui un mejor tablero, me guardo su valor y movimientos
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

    bool valid_positions(const std::vector<player_status>& team, int i, int j, int k) {
        //verifico que todos los jugaores esten dentro de la cancha y que ninguno comparta posicion
        return inside_board(team[0], i) && inside_board(team[1], j) && inside_board(team[2], k) && in_different_positions(team, i, j, k);
    }

    bool inside_board(const player_status &player, int dir) {
        int i = player.i + moves[dir].first;
        int j = player.j + moves[dir].second;

        return i >= 0 && j >= 0 && i < rows && j < columns;
    }

    bool in_different_positions(const vector<player_status>& team, int i, int j, int k) {
        return !(in_same_position(team[0], i, team[1], j) || in_same_position(team[0], i, team[2], k) || in_same_position(team[1], j, team[2], k));
    }

    bool in_same_position(const player_status& p1, int dir1, const player_status& p2, int dir2) {
        //le sumo a la posicion del jugador el movimiento que realiza
        std::pair<int, int> new_p1_pos = make_pair(p1.i + moves[dir1].first, p1.j + moves[dir1].second);
        std::pair<int, int> new_p2_pos = make_pair(p2.i + moves[dir2].first, p2.j + moves[dir2].second);

        return new_p1_pos == new_p2_pos;
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
