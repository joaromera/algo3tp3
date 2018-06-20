#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "constants.hpp"
#include "ball.h"

using namespace std;

const vector< tuple<int, int > > movimientos = {
  make_tuple(-1, -1),
  make_tuple(-1, 0),
  make_tuple(-1, 1),
  make_tuple(0, 1),
  make_tuple(1, 1),
  make_tuple(1, 0),
  make_tuple(1, -1),
  make_tuple(0, -1),
  make_tuple(0, 0)
};

class Player{

  public:
    Player(int & player_id, double & p_quite);
    ~Player();
    void move(int & direction);
    void backwardMove(int & direction);
    void undoMove();
    void moveBall();
    void takeBall(Ball* ball);
    void printPlayer();

    int id;
    double p_quite;
    tuple<int, int> old_position;
    int i;
    int j;
    Ball* ball;
};

#endif //PLAYER_H