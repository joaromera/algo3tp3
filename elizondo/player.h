#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "board_status.hpp"
#include "constants.hpp"
#include "ball.h"

struct Player
{
    int id;
    double p_quite;
    std::tuple<int, int> old_position;
    int i;
    int j;
    Ball* ball;

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
      old_position = std::make_tuple(-1, -1);
      ball = nullptr;
    }

    void move(const int &direction)
    {
      old_position = std::make_tuple(i, j);
      i += std::get<0>(moves[direction]);
      j += std::get<1>(moves[direction]);
      moveBall();
    }

    void backwardMove(const int &direction)
    {
      old_position = std::make_tuple(i, j);
      i -= std::get<0>(moves[direction]);
      j -= std::get<1>(moves[direction]);
      moveBall();
    }

    void undoMove()
    {
      if (std::get<0>(old_position) != -1 && std::get<1>(old_position) != -1)
      {
        i = std::get<0>(old_position);
        j = std::get<1>(old_position);
        old_position = std::make_tuple(-1, -1);
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
      const std::string with_ball = "CON_PELOTA";
      const std::string without_ball = "SIN_PELOTA";
      const std::string possession = ball != nullptr ? with_ball : without_ball;
      std::cout << id << " " << i << " " << j << " " << possession << std::endl;
    }
};
