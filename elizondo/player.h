#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <random>
#include <vector>
#include <map>

using namespace std;

const vector<tuple<int, int > > movimientos = {
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
    void move(int & direction);
    void backwardMove(int & direction);
    void undoMove();
    void moveBall();
    void takeBall(tuple<int, int > & ball);
    void printPlayer();

  private:
    int id;
    double p_quite;

    //tipo position? tipo dire?
    tuple<int, int> old_position;
    int i;
    int j;

    // Ball*
    tuple<int, int> ball;
};

#endif //PLAYER_H