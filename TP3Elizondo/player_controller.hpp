#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "board_status.hpp"
#include "constants.hpp"

void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

void send(double msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == EXIT) {
        send(DONE);
        std::exit(0);
    }
    return std::stoi(msg);
}

int read_double() {
    std::string msg;
    std::cin >> msg;
    if (msg == EXIT) {
        send(DONE);
        std::exit(0);
    }
    return std::stod(msg);
}

std::string read_str() {
    std::string msg;
    std::cin >> msg;
    if (msg == EXIT) {
        send(DONE);
        std::exit(0);
    }
    return msg;
}

template<typename TEAM>
class player_controller {
    TEAM team;
    int players_per_team = 3;
    int steps;
    std::ofstream log_file;

public:
    player_controller(std::string team_name) {

        // Lee los datos generales del juego
        int columns = read_int();
        int rows = read_int();
        this->steps = read_int();

        // Lee de que lado juega el equipo
        std::string side = read_str();


        // Lee los jugadores del equipo
        std::vector<player> players;
        for (int i = 0; i < this->players_per_team; i++) {
            players.emplace_back(read_int(), read_double());
        }

        // Lee los jugadores del equipo contrario
        std::vector<player> oponent_players;
        for (int i = 0; i < this->players_per_team; i++) {
            oponent_players.emplace_back(read_int(), read_double());
        }

        this->team = TEAM(columns, rows, this->steps, side, players, oponent_players);
        
        this->log_file.open ("log/cpp_log_" + team_name + ".log");
        this->log_file << "Parameters: " << columns << " " << rows << " " << this->steps << std::endl;

        std::vector<player_status> initial_positions;
        this->team.starting_positions(initial_positions);
        this->send_positions(initial_positions);
    };

    void play() {
        board_status current_board;
        std::vector<player_move> moves;
        
        for (int step = 0; step < this->steps; step++) {
            this->log_file << "Step: " << step << std::endl;
            current_board.clear();
            this->read_board_status(current_board);
            this->team.make_move(current_board, moves);
            this->send_move(moves);
        }

        this->team.finish(read_str());
    }

    void read_board_status(board_status& current_board) {
        int i, player_id, player_i, player_j;
        bool in_posetion, free_ball;

        free_ball = true;

        // Lee el estatus de los jugadores del equipo
        for (i = 0; i < this->players_per_team; i++) {
            player_id = read_int();
            player_i = read_int();
            player_j = read_int();
            in_posetion = read_str() == IN_POSETION;
            current_board.team.emplace_back(player_id, player_i, player_j, in_posetion);
            free_ball = free_ball && !in_posetion;
            //this->log_file << player_id + ' ' + player_i + ' ' + player_j + ' ' + in_posetion << ' ';
        }

        // Lee el estatus de los jugadores del equipo oponente
        for (i = 0; i < this->players_per_team; i++) {
            player_id = read_int();
            player_i = read_int();
            player_j = read_int();
            in_posetion = read_str() == IN_POSETION;
            current_board.oponent_team.emplace_back(player_id, player_i, player_j, in_posetion);
            free_ball = free_ball && !in_posetion;
            this->log_file << "player_id: " << player_id << std::endl;
        }

        // Si la pelota está libre, lee el estatus de la peluta
        if (free_ball) {
            current_board.ball.i = read_int();
            current_board.ball.j = read_int();
            current_board.ball.dir = read_int();
            current_board.ball.steps = read_int();
            current_board.ball.is_free = true;
        }
    }

    void send_move(const std::vector<player_move>& moves) {
        std::string str_move = "";

        for(auto& m : moves) {
            str_move += std::to_string(m.player_id) + ' ' + m.move_type + ' ' + std::to_string(m.dir);
            if (m.move_type == PASE) {
                str_move += ' ' + std::to_string(m.steps);
            }
            str_move += ' ';
        }


        str_move.pop_back();
        this->log_file << "send move: " << str_move << std::endl;

        send(str_move);
    }

    void send_positions(const std::vector<player_status>& positions) {
        std::string str_positions = "";

        for(auto& p : positions) {
            str_positions += std::to_string(p.id) + ' ' + std::to_string(p.i) + ' ' + std::to_string(p.j);
            str_positions += ' ';
        }

        str_positions.pop_back();
        this->log_file << "send position: " << str_positions << std::endl;

        send(str_positions);
    }    
};

#endif //PLAYER_CONTROLLER