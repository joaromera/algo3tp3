#pragma once

#include <iostream>
#include <vector>

#include "board_status.hpp"
#include "constants.hpp"
#include "ball.h"

struct Player
{
    int id;
    double p_quite;
    tuple<int, int> old_position;
    int i;
    int j;
    Ball* ball;

    Player(const player &player)
    {
      id = player.id;
      p_quite = player.probability;
      old_position = make_tuple(-1, -1);
      ball = nullptr;
    }

    ~Player()
    {
      if (ball != nullptr)
      {
        delete ball;
      }
    }

    Player(const int &pPlayerId, const double &pPQuite)
    {
      id = pPlayerId;
      p_quite = pPQuite;
      old_position = make_tuple(-1, -1);
      ball = nullptr;
    }

    void move(const int &direction)
    {
      old_position = make_tuple(i, j);
      i += get<0>(moves[direction]);
      j += get<1>(moves[direction]);
      moveBall();
    }

    void backwardMove(const int &direction)
    {
      old_position = make_tuple(i, j);
      i -= get<0>(moves[direction]);
      j -= get<1>(moves[direction]);
      moveBall();
    }

    void undoMove()
    {
      if (get<0>(old_position) != -1 && get<1>(old_position) != -1)
      {
        i = get<0>(old_position);
        j = get<1>(old_position);
        old_position = make_tuple(-1, -1);
        moveBall();
      }
    }

    void moveBall()
    {
      if (ball)
      {
        ball->i = i;
        ball->j = j;
      }
    }

    void takeBall(Ball* pBall)
    {
      ball = pBall;
      ball->setMovement(-1, -1);
      moveBall();
    }

    void printPlayer()
    {
      string with_ball = "CON_PELOTA";
      string without_ball = "SIN_PELOTA";
      string possession = ball != nullptr ? with_ball : without_ball;
      cout << id << " " << i << " " << j << " " << possession << endl;
    }
};
