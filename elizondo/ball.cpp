#include <string>
#include <random>
#include <vector>
#include <map>

#include "ball.h"

using namespace std;
    
Ball::Ball() {
    get<0>(this->movement) = -1;
    get<1>(this->movement) = -1;
}

Ball::~Ball() {}

void Ball::setMovement(tuple<int,int> movement) {
    this->movement = movement;
}

bool Ball::isStill() {
    return get<0>(this->movement) == -1 && get<1>(this->movement) == -1;
}

void Ball::move() {
    if (this->isStill()) {
        return;
    }

    if (get<1>(this->movement) > 0) {
        pair<int,int> move = moves[get<0>(this->movement)];
        this->i += 2 * move.first;
        this->j += 2 * move.second;
        get<1>(this->movement) = get<1>(this->movement) - 1;
    }
}

tuple<int,int> Ball::finalPosition() {
    pair<int,int> move = moves[get<0>(this->movement)];
    int steps = 2 * get<1>(this->movement);
    return make_tuple(this->i + steps * move.first, this->j + steps * move.second);
}

vector<tuple<int,int>> Ball::trajectory() {
    pair<int,int> move = moves[get<0>(this->movement)];
    int steps = 2 * get<1>(this->movement);
    vector<tuple<int,int>> trajectory;
    for(int i = 0; i <= steps; i++) {
        trajectory.push_back(make_tuple(this->i + this-> i * move.first, this->j + i * move.second));
    }
    return trajectory;
}

void Ball::undoMove() {
    if (this->isStill()) {
        return;
    }

    pair<int,int> move = moves[get<0>(this->movement)];
    this->i -= 2 * move.first;
    this->j -= 2 * move.second;
    get<1>(this->movement) = get<1>(this->movement) + 1;
}

void Ball::step_back_one() {
    if (this->isStill()) {
        return;
    }
    
    pair<int,int> move = moves[get<0>(this->movement)];
    this->i -= move.first;
    this->j -= move.second;
}