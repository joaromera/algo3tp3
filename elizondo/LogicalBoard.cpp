#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "constants.hpp"
#include "player.h"
#include "ball.hpp"

using namespace std;

class LogicalBoard {
	
	int scoreA, scoreB, columns, rows;
	vector<Player> playersA, playersB;
    vector<tuple< int, int> > goalA;
	vector<tuple< int, int> > goalB;
    Ball* free_ball;
    LogicalBoard lastState;

	LogicalBoard();

	LogicalBoard(
			int columns,
			int rows,
			vector<Player>& playersA,
			vector<Player>& playersB
	) {
		this->columns = columns;
		this->rows = rows;
		this->scoreA = 0;
		this->scoreB = 0;
		this->goalA = CreateGoal(-1);
		this->goalB = CreateGoal(this->columns);

		this->playersA = playersA;
		this->playersB = playersB;

		this->free_ball = nullptr;
	}

    void makeTeamMove(vector<Player> &team, vector<move> &moves) {
        
    };

    double normalize(double &prob_1, double &prob_2) {
        double total = prob_1 + prob_2;
        return prob_2 / total; //en LogicalBoard.py siempre usan el prob_2
    }

    void fightBall(Player &p_ball, Player &p_empty) {
        double prob_ball = 1 - p_ball->p_quite;
        double prob_empty = p_empty->p_quite;

        prob_empty = normalize(prob_ball, prob_empty);
        
        double random_prob = (rand() % 101) / 100;

        if (random_prob <= prob_empty) {
            p_empty->takeBall(p_ball->ball);
            p_ball->ball = nullptr;
        }
    }

    void fairFightBall(Player &p1, Player &p2) {
        double prob_p2 = normalize(p1.p_quite, p2.p_quite);
        double random_prob = (rand() % 101) / 100;

        if (random_prob < prob_p2) {
            p2.takeBall(free_ball);
        } else {
            p1.takeBall(free_ball);
        }

        free_ball = nullptr;
    }

    bool intercepted() {
        bool result = True;

        // método para interceptar la pelota

        return result;
    }

    void makeMove() {}

    void undoMove() {}

    string winner() {
        if (scoreA > scoreB) {
            return "A"
        } else if (scoreB > scoreA) {
            return "B"
        } else {
            return "Empate"
        }
    }

    void updateScore() { // no sé si es void
        // si nadie tiene la pelota retorna None ¿?
        // si la pelota está en el arco de A, le suma un gol a B y devuelve "A"
        // si la pelota está en el arco de B, le suma un gol a A y devuelve "B"
    }

    void reset() {}
        // mueve a los jugadores a las posiciones iniciales
        // pone los scores en 0 para ambos equipos
    
    bool positionInBoard(int i, int j) {
        return 0 <= i && i < rows && 0 <= j && j < columns;
    }

    void startingPositions(position_A, position_B, const string &starting) {
        // chequea que las posiciones iniciales sean en los lados correctos de la cancha
        // busca al jugador con la pelota y se la saca

        free_ball = nullptr;

        // coloca los jugadores en las posiciones correctas

        // le doy la pelota al jugador que saca y lo pongo en el centro
        if (starting == "A") {

        } else {

        }
    }

    void getState() {
        tuple<int, int> ball_position;
        if (free_ball != nullptr) {
            ball_position.first = free_ball->i;
            ball_position.second = free_ball->j;

        }

        // devuelve las posiciones de los jugadores sus ids sus prob de quite
        // y la posicion de la pelota
    }

    void getGoal() {
        // devuelve posicion del arco segun el equipo
    }

    void getTeam() {
        // devuelve el team_A o team_B
    }

    void print() {
        // invoca el str de cada jugdador
        // si la pelota está libre agrega el str de la pelota
    }
	
    vector< tuple<int, int> > CreateGoal(int col) {
		vector< tuple<int, int> > goal;
		int floor = (this->rows / 2) - 1;

		for (int i = 0; i < 3; ++i) {
			goal.push_back(make_tuple(floor + i, col));
		}

		return goal;
	}

};