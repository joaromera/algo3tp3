#ifndef BALL_H
#define BALL_H

#include <string>
#include <random>
#include <vector>
#include <map>

#include "constants.hpp"

using namespace std;

class Ball {
    public:
    Ball();
    virtual ~Ball();
    void setMovement(tuple<int,int> movement);
    bool isStill();
    void move();
    tuple<int,int> finalPosition();
    vector<tuple<int,int> > trajectory();
    void undoMove();
    void step_back_one();

    int i;
    int j;
    tuple < int, int > movement;
};

#endif //BALL_H