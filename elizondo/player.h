#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>

#include "board_status.hpp"
#include "constants.hpp"
#include "ball.h"

using namespace std;

class Player{

  public:

    int id;
    double p_quite;
    tuple<int, int> old_position;
    int i;
    int j;
    Ball* ball;

    Player(player player) {
      this->id = player.id;
      this->p_quite = player.probability;
      this->old_position = make_tuple(-1, -1);
      this->ball = nullptr;
    };

    ~Player();

    Player(int &player_id, double &p_quite) {
      this->id = player_id;
      this->p_quite = p_quite;
      this->old_position = make_tuple(-1, -1);
      this->ball = nullptr;
    };

    void move(int &direction) {
      this->old_position = make_tuple(this->i, this->j);
      this->i += get<0>(moves[direction]);
      this->j += get<1>(moves[direction]);
      moveBall();
    }

    void backwardMove(int &direction) {
      this->old_position = make_tuple(this->i, this->j);
      this->i -= get<0>(moves[direction]);
      this->j -= get<1>(moves[direction]);
      moveBall();
    };

    void undoMove() {
      if (get<0>(old_position) != -1 && get<1>(old_position) != -1) {
        this->i = get<0>(old_position);
        this->j = get<1>(old_position);
        this->old_position = make_tuple(-1, -1);
        moveBall();
      }
    }

    void moveBall() {
      if (ball != nullptr) {
        this->ball->i = this->i;
        this->ball->j = this->j;
      }
    }

    void takeBall(Ball* ball) {
      this->ball = ball;
      this->ball->setMovement(-1, -1);
      moveBall();
    }

    void printPlayer() {
      string with_ball = "CON_PELOTA";
      string without_ball = "SIN_PELOTA";
      string possession = this->ball != nullptr ? without_ball : with_ball;
      cout << id << " " << i << " " << j << " " << possession << endl;
    }

};

#endif //PLAYER_H