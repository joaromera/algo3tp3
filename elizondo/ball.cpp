#include <string>
#include <random>
#include <vector>
#include <map>

using namespace std;

array < pair < int, int >, 9 > moves = { 
    make_pair(0,0), 
    make_pair(-1,-1), 
    make_pair(-1,0), 
    make_pair(-1,1), 
    make_pair(0,1), 
    make_pair(1,1), 
    make_pair(1,0), 
    make_pair(1,-1), 
    make_pair(0,-1) 
};

class Ball {

  public:

    int i;
	int j;
    tuple < int, int > movement;

    Ball() {
        get<0>(this->movement) = -1;
        get<1>(this->movement) = -1;
    }

    virtual ~Ball() {
    }

    void setMovement(tuple<int,int> movement) {
        this->movement = movement;
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
    
    
};