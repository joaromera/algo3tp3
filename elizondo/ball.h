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
        int i;
        int j;
        tuple < int, int > movement;

        Ball() {
            get<0>(this->movement) = -1;
            get<1>(this->movement) = -1;
        }

        Ball(int i, int j) {
            this->i = i;
            this->j = j;
        }

        virtual ~Ball() {}

        void setMovement(int dir, int steps) {
            this->movement = make_tuple(dir, steps);
        }

        bool isStill() {
            return get<0>(this->movement) == -1 && get<1>(this->movement) == -1;
        }

        void move() {
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

        tuple<int,int> finalPosition() {
            pair<int,int> move = moves[get<0>(this->movement)];
            int steps = 2 * get<1>(this->movement);
            return make_tuple(this->i + steps * move.first, this->j + steps * move.second);
        }

        vector< tuple<int,int> > trajectory() {
            pair<int,int> move = moves[get<0>(this->movement)];
            int steps = 2 * get<1>(this->movement);
            vector<tuple<int,int>> trajectory;
            for(int i = 0; i <= steps; i++) {
                trajectory.push_back(make_tuple(this->i + this-> i * move.first, this->j + i * move.second));
            }
            return trajectory;
        }

        void undoMove() {
            if (this->isStill()) {
                return;
            }

            pair<int,int> move = moves[get<0>(this->movement)];
            this->i -= 2 * move.first;
            this->j -= 2 * move.second;
            get<1>(this->movement) = get<1>(this->movement) + 1;
        }

        void step_back_one() {
            if (this->isStill()) {
                return;
            }
            
            pair<int,int> move = moves[get<0>(this->movement)];
            this->i -= move.first;
            this->j -= move.second;
        }

};

#endif //BALL_H