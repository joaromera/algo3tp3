#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "player.h"
#include "ball.h"

using namespace std;

class LogicalBoard {

    int scoreA, scoreB, columns, rows;
    vector < Player > playersA, playersB;
    vector < tuple < int, int > > goalA;
    vector < tuple < int, int > > goalB;
    Ball * free_ball;
    vector < player_status > initial_positions;
    // LogicalBoard lastState;

    LogicalBoard();

    LogicalBoard(
        int columns,
        int rows,
        vector < Player > & playersA,
        vector < Player > & playersB,
        vector < player_status > initial_positions
    ) {
        this->columns = columns;
        this->rows = rows;
        this->scoreA = 0;
        this->scoreB = 0;
        this->goalA = CreateGoal(-1);
        this->goalB = CreateGoal(this->columns);

        this->playersA = playersA;
        this->playersB = playersB;
        this->initial_positions = initial_positions;
        this->free_ball = nullptr;
    }

    // void makeTeamMove(vector<Player> &team, vector<move> &moves) {

    // };

    double normalize(double & prob_1, double & prob_2) {
        double total = prob_1 + prob_2;
        return prob_2 / total; //en LogicalBoard.py siempre usan el prob_2
    }

    void fightBall(Player & p_ball, Player & p_empty) {
        double prob_ball = 1 - p_ball.p_quite;
        double prob_empty = p_empty.p_quite;

        prob_empty = normalize(prob_ball, prob_empty);

        double random_prob = (rand() % 101) / 100;

        if (random_prob <= prob_empty) {
            p_empty.takeBall(p_ball.ball);
            p_ball.ball = nullptr;
        }
    }

    void fairFightBall(Player & p1, Player & p2) {
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
        bool result = true;

        // método para interceptar la pelota

        return result;
    }

    void makeMove() {}

    void undoMove() {}

    string winner() {
        if (scoreA > scoreB) {
            return "A";
        } else if (scoreB > scoreA) {
            return "B";
        } else {
            return "Empate";
        }
    }

    void updateScore() { // no sé si es void
        Ball * ball = free_ball;

        if (ball == nullptr) {
            for (auto p: playersA) {
                if (p.ball != nullptr) {
                    ball = p.ball;
                }
            }

            for (auto p: playersB) {
                if (p.ball != nullptr) {
                    ball = p.ball;
                }
            }
        }

        for (auto g: goalA) { // si la pelota está en el arco de A, le suma un gol a B y devuelve "A"
            if (get < 0 > (g) == ball->i && get < 0 > (g) == ball->j) {
                scoreB++;
                // return A; // ?
            }
        }

        for (auto g: goalB) { // si la pelota está en el arco de B, le suma un gol a A y devuelve "B"
            if (get < 0 > (g) == ball->i && get < 0 > (g) == ball->j) {
                scoreA++;
                // return B; // ?
            }
        }
    }

    bool positionInBoard(int i, int j) {
        return 0 <= i && i < rows && 0 <= j && j < columns;
    }

    void getState() {
        tuple < int, int > ball_position;
        if (free_ball != nullptr) {
            get < 0 > (ball_position) = free_ball->i;
            get < 1 > (ball_position) = free_ball->j;

        }

        // devuelve las posiciones de los jugadores sus ids sus prob de quite
        // y la posicion de la pelota
    }

    vector < tuple < int, int > > getGoal(const string & team) {
        vector < tuple < int, int > > goal;
        if (team == "A") {
            goal = goalA;
        } else if (team == "B") {
            goal = goalB;
        }
        return goal;
    }

    void print() {
        // invoca el str de cada jugdador
        // si la pelota está libre agrega el str de la pelota
    }

    vector < tuple < int, int > > CreateGoal(int col) {
        vector < tuple < int, int > > goal;
        int floor = (this->rows / 2) - 1;

        for (int i = 0; i < 3; ++i) {
            goal.push_back(make_tuple(floor + i, col));
        }

        return goal;
    }

};
