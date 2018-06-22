#include <string>
#include <random>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"

std::random_device rd;
std::mt19937 generator(rd());

class greedier_player {
    
    int columns, rows, steps;
    std::string team, side;
    std::vector<std::pair <int, int> > own_goal;
    std::vector<std::pair <int, int> > opponnent_goal;
    const vector<player>* players;
    const vector<player>* opponnents;

public:

    greedier_player() = default;

    greedier_player(
        int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<player>& players, // Este no lo usa para nada
        const std::vector<player>& opponnent_players // Este no lo usa para nada
    ) {
        this->columns = columns;
        this->rows = rows;
        this->steps = steps;
        this->side = side;
        this->team = team;
        this->players = &players;
        this->opponnents = &opponnent_players;
        this->get_goal_positions();
    }

    void get_goal_positions() {
        int mid_row = this->rows / 2;
        int column_own_goal = this->columns;
        int column_opoent_goal = -1;
        if (this->side == IZQUIERDA) {
            column_own_goal = -1;
            int column_opoent_goal = this->columns;
        }

        for (int i = -1; i < 2; i++) {
            own_goal.push_back(std::make_pair(i, column_own_goal));
            opponnent_goal.push_back(std::make_pair(i, column_opoent_goal));
        }

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

        LogicalBoard* logical_board = new LogicalBoard(
            this->columns,
            this->rows,
            *this->players,
            *this->opponnents,
            current_board
        );

        int board_score = evaluate_board(current_board);
        
        std::string fileName = "board.log";
        std::ofstream f;
        f.open(fileName, std::fstream::out);
        f << board_score << std::endl;
        f.close();
    }

    int evaluate_board(const board_status& current_board) {
        int result = 0;
        // Mientras más cerca esté la pelota del arco contrario, mejor
        result -= distance_ball_opponnent_goal(current_board);

        if (who_has_the_ball(current_board) == "GREEDY") {
            result++;
            // Queremos que se acerquen al arco contrario y que se alejen de los oponentes
            for (auto p : current_board.team) {
                result -= distance_player_opponnent_goal(current_board, p.id);
                result += distance_player_closest_opponnent(current_board, p.id);
            }

        } else if (who_has_the_ball(current_board) == "OPPONNENT") {
            result--;
            // Queremos que se acerquen a los contrarios
            for (auto p : current_board.team) {
                result -= distance_player_closest_opponnent(current_board, p.id);
            }
        } else {
            // Queremos que se acerquen a la pelota porque está libre
            for (auto p : current_board.team) {
                result -= distance_player_ball(current_board, p.id);
            }   
        }

        return result;
    }

    double distance(int from_i, int from_j, int to_i, int to_j) {
        double x = static_cast<double>(from_i) - static_cast<double>(to_i);
        double y = static_cast<double>(from_j) - static_cast<double>(to_j);
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    // Queremos que sea bajo
    int distance_ball_opponnent_goal(const board_status& current_board) {
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

        int di1 = distance(ball_i, ball_j, this->opponnent_goal[0].first, this->opponnent_goal[0].second);
        int di2 = distance(ball_i, ball_j, this->opponnent_goal[1].first, this->opponnent_goal[1].second);
        int di3 = distance(ball_i, ball_j, this->opponnent_goal[2].first, this->opponnent_goal[2].second);
    
        return std::min(di1,std::min(di2,di3));
    }

    // Si tenemos la pelota queremos que sea grande, si no la tenemos queremos que sea baja
    int distance_player_closest_opponnent(const board_status& current_board, int player_id)  {
        int player_i;
        int player_j;
        for (auto p : current_board.team) {
            if (p.id == player_id) {
                player_i = p.i;
                player_j = p.j;
            }
        }
        int di1 = distance(player_i, player_j, current_board.oponent_team[0].i, current_board.oponent_team[0].j);
        int di2 = distance(player_i, player_j, current_board.oponent_team[1].i, current_board.oponent_team[1].j);
        int di3 = distance(player_i, player_j, current_board.oponent_team[2].i, current_board.oponent_team[2].j);
    
        return std::min(di1,std::min(di2,di3));
    }

    // Con la pelota queremos que sea bajo
    int distance_player_opponnent_goal(const board_status& current_board, int player_id)  {
        int player_i;
        int player_j;
        for (auto p : current_board.team) {
            if (p.id == player_id) {
                player_i = p.i;
                player_j = p.j;
            }
        }
        int di1 = distance(player_i, player_j, this->opponnent_goal[0].first, this->opponnent_goal[0].second);
        int di2 = distance(player_i, player_j, this->opponnent_goal[1].first, this->opponnent_goal[1].second);
        int di3 = distance(player_i, player_j, this->opponnent_goal[2].first, this->opponnent_goal[2].second);
    
        return std::min(di1,std::min(di2,di3));
    }

    // Si no tenemos la pelota queremos que sea chico
    int distance_player_ball(const board_status& current_board, int player_id)  {
        int player_i;
        int player_j;
        for (auto p : current_board.team) {
            if (p.id == player_id) {
                player_i = p.i;
                player_j = p.j;
            }
        }
        int di1 = distance(player_i, player_j, current_board.ball.i, current_board.ball.j);
        int di2 = distance(player_i, player_j, current_board.ball.i, current_board.ball.j);
        int di3 = distance(player_i, player_j, current_board.ball.i, current_board.ball.j);
    
        return std::min(di1,std::min(di2,di3));
    }

    std::string who_has_the_ball(const board_status& current_board) {
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

    void finish(std::string result) { }
};