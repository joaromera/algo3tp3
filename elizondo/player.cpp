
#include <iostream>
#include "player.h"


Player::Player(int &player_id, double &p_quite) {
  this->id = player_id;
  this->p_quite = p_quite;
  this->old_position = make_tuple(-1, -1);
  this->ball = make_tuple(-1, -1);
}

void Player::move(int &direction) {
  this->old_position = make_tuple(this->i, this->j);
  this->i += get<0>(movimientos[direction]);
  this->j += get<1>(movimientos[direction]);
  moveBall();
}

void Player::backwardMove(int &direction) {
  this->old_position = make_tuple(this->i, this->j);
  this->i -= get<0>(movimientos[direction]);
  this->j -= get<1>(movimientos[direction]);
  moveBall();
}

void Player::undoMove() {
  if (get<0>(old_position) != -1 && get<1>(old_position) != -1) {
    this->i = get<0>(old_position);
    this->j = get<1>(old_position);
    this->old_position = make_tuple(-1, -1);
    moveBall();
  }
}

void Player::moveBall() {

  // esta mal el metodo, tiene que user los metodos de Ball
  if (get<0>(ball) != 1 && get<1>(ball) != 1) {
    get<0>(this->ball) = this->i;
    get<1>(this->ball) = this->j;
  }
  /*
  if not self.ball is None:
  self.ball.i = self.i
  self.ball.j = self.j
  */
}

//cambiar por el tipo Ball
void Player::takeBall(tuple<int, int> & ball) {
  this->ball = ball;
  moveBall();
  // this->ball.setMovement(-1, -1); ???
  /*
   self.ball = ball
   self.ball.setMovement(None)
   self.moveBall()
   */
}

void Player::printPlayer() {
  string with_ball = "CON_PELOTA";
  string without_ball = "SIN_PELOTA";
  string possession = get<0>(this->ball) == -1 && get<1>(this->ball) == -1
      ? without_ball
      : with_ball;
  cout << id << " " << i << " " << j << " " << possession << endl;
}
