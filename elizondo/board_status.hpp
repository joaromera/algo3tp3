#ifndef BOARD_STATUS
#define BOARD_STATUS

struct player {
    int id;
    double probability;

    player(int id, double probability) : id(id), probability(probability) {}
};

struct move {
    int number;
    int i;
    int j;

    move(std::initializer_list<int> params) {
        auto it = params.begin();
        this->number = *it; ++it;
        this->i = *it; ++it;
        this->j = *it;
    }
};

struct player_status {
    int id;
    int i;
    int j;
    bool in_posetion = false;

    player_status(int id) : id(id) {}
    player_status(int id, int i, int j, bool in_posetion) : id(id), i(i), j(j), in_posetion(in_posetion) {}
};

struct ball_status {
    int i;
    int j;
    int dir;
    int steps;
    bool is_free = false;
};

struct player_move {
    int player_id;
    std::string move_type;
    int dir;
    // Solo sirve para cuando el tipo de movimiento es un pase e indica cuanta
    // fuerza tiene el pase (i.e. cuan lejos llegará el balón si nadie lo intercepta)
    int steps; 
};

struct board_status {
    std::vector<player_status> team;
    std::vector<player_status> oponent_team;
    ball_status ball;

    void clear() {
        team.clear();
        oponent_team.clear();
        ball.is_free = false;
    }
};

#endif //BOARD_STATUS