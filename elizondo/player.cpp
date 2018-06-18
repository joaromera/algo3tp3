#include <iostream>
#include "player.h"

Player::Player(int &player_id, double &p_quite) {
  this->id = player_id;
  this->p_quite = p_quite;
  this->old_position = make_tuple(-1, -1);
  this->ball = nullptr;
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
  if (ball != nullptr) {
    this->ball->i = this->i;
    this->ball->j = this->j;
  }
}

//cambiar por el tipo Ball
void Player::takeBall(Ball* ball) {
  this->ball = ball;
  this->ball->setMovement(make_tuple(-1, -1));
  moveBall();
}

void Player::printPlayer() {
  string with_ball = "CON_PELOTA";
  string without_ball = "SIN_PELOTA";
  string possession = this->ball != nullptr ? without_ball : with_ball;
  cout << id << " " << i << " " << j << " " << possession << endl;
}

Player::~Player() {
}
