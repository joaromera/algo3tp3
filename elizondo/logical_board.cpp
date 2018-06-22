#include <initializer_list>
#include <string>
#include <random>
#include <vector>
#include <map>
#include <stdbool.h>

#include "board_status.hpp"
#include "player.h"
#include "ball.h"

using namespace std;

class LogicalBoard {

    public:
    
        int scoreA, scoreB, columns, rows;
        vector < Player* > teamA, teamB;
        vector < tuple < int, int > > goalA;
        vector < tuple < int, int > > goalB;
        Ball* free_ball;
        board_status* last_state;
        board_status* new_state;

        LogicalBoard(
            int columns,
            int rows,
            vector < player > teamA,
            vector < player > teamB
        ) {
            this->scoreA = 0;
            this->scoreB = 0;

            vector < Player* > _teamA;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(teamA[i]);
                _teamA.push_back(aux);
            }
            this->teamA = _teamA;

            vector < Player* > _teamB;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(teamB[i]);
                _teamB.push_back(aux);
            }
            this->teamB = _teamB;

            this->columns = columns;
            this->rows = rows;

            this->goalA = CreateGoal(-1);
            this->goalB = CreateGoal(this->columns);

            this->free_ball = new Ball();
            this->last_state = nullptr;
            this->new_state = nullptr;
        }

        LogicalBoard( // con board status
            int columns,
            int rows,
            vector < player > teamA,
            vector < player > teamB,
            board_status status
        ) {
            this->columns = columns;
            this->rows = rows;
            this->scoreA = 0;
            this->scoreB = 0;
            this->goalA = CreateGoal(-1);
            this->goalB = CreateGoal(this->columns);
            this->free_ball = new Ball();
            this->last_state = nullptr;
            this->new_state = nullptr;

            vector < Player* > _teamA;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(teamA[i]);
                _teamA.push_back(aux);
            }
            this->teamA = _teamA;

            vector < Player* > _teamB;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(teamB[i]);
                _teamB.push_back(aux);
            }
            this->teamB = _teamB;

            for (auto ps : status.team) {
                for (auto p : this->teamA) {
                    if (p->id == ps.id) {
                        p->i = ps.i;
                        p->j = ps.j;
                        if (ps.in_posetion) {
                            p->ball = this->free_ball;
                            this->free_ball = nullptr;
                        }
                    }
                }
            }
            for (auto ps : status.oponent_team) {
                for (auto p : this->teamB) {
                    if (p->id == ps.id) {
                        p->i = ps.i;
                        p->j = ps.j;
                        if (ps.in_posetion) {
                            p->ball = this->free_ball;
                            this->free_ball = nullptr;
                        }
                    }
                }
            }
            if (this->free_ball != nullptr) {
                this->free_ball->i = status.ball.i;
                this->free_ball->j = status.ball.j;
                this->free_ball->movement = make_tuple(status.ball.dir, status.ball.steps);
            }
        }

        void makeTeamMove(vector<Player*> *team, vector<player_move> &moves) {
            for (int i = 0; i < 3; i++) {
                Player* aPlayer = (*team)[i];
                player_move aMove = moves[aPlayer->id];

                if (aMove.move_type == "MOVIMIENTO") {
                    aPlayer->move(aMove.dir);
                }

                if (aMove.move_type == "PASE") {
                    this->free_ball = aPlayer->ball;
                    this->free_ball->setMovement(aMove.dir, aMove.steps);
                    aPlayer->ball = nullptr;
                }
            }
        };

        double normalize(double & prob_1, double & prob_2) {
            double total = prob_1 + prob_2;
            return prob_2 / total;
        }

        void fightBall(Player & p_ball, Player & p_empty) {
            double prob_ball = 1 - p_ball.p_quite;
            double prob_empty = p_empty.p_quite;

            prob_empty = normalize(prob_ball, prob_empty);

            double random_prob = (rand() % 101) / 100;

            if (random_prob <= prob_empty) {
                p_empty.takeBall(p_ball.ball);
                p_ball.ball = nullptr;
            }
        }

        void fairFightBall(Player & p1, Player & p2) {
            double prob_p2 = normalize(p1.p_quite, p2.p_quite);
            double random_prob = (rand() % 101) / 100;

            if (random_prob < prob_p2) {
                p2.takeBall(this->free_ball);
            } else {
                p1.takeBall(this->free_ball);
            }

            this->free_ball = nullptr;
        }

        bool intercepted(Player* player, bool isOponent) {

            bool result = true;
            
            player_status prevStatePlayer;
            vector<player_status> team;

            if (isOponent) {
                team = this->last_state->oponent_team;
            } else {
                team = this->last_state->team;
            }

            for(int i; i < 3; i++) {
                if (player->id == team[i].id) {
                    prevStatePlayer = team[i];
                }
            }

            result = result 
                && prevStatePlayer.i == player->i 
                && prevStatePlayer.j == player->j;

            player->backwardMove(get<0>(this->free_ball->movement));

            result = result 
                && player->i == this->free_ball->i 
                && player->j == this->free_ball->j;
            player->undoMove();

            return result;
        }

        vector < Player* > makeMove(vector< player_move > movesA, vector< player_move > movesB) {
            this->getState();
            this->makeTeamMove(&(this->teamA), movesA);
            this->makeTeamMove(&(this->teamB), movesB);
            
            if (this->free_ball != nullptr) {
                
                vector< Player* > intercepters;
                for (int i = 0; i < 3; i++) {
                    if (this->intercepted(this->teamA[i], false)) {
                        intercepters.push_back(this->teamA[i]);
                    }
                }
                for (int i = 0; i < 3; i++) {
                    if (this->intercepted(this->teamB[i], true)) {
                        intercepters.push_back(this->teamB[i]);
                    }
                }
                
                if (intercepters.size() == 1) {
                    intercepters[0]->takeBall(this->free_ball);
                    this->free_ball = nullptr;
                } else if (intercepters.size() == 2) {
                    this->fairFightBall(*intercepters[0], *intercepters[1]);
                } else {
                    this->free_ball->move();
                    if (this->positionInBoard(this->free_ball->i, this->free_ball->j)) {
                        vector< Player* > playersToFight;
                        for (int i = 0; i < 3; i++) {
                            if (this->teamA[i]->i == this->free_ball->i 
                                && this->teamA[i]->j == this->free_ball->j) {
                                playersToFight.push_back(this->teamA[i]);
                            }
                        }
                        for (int i = 0; i < 3; i++) {
                            if (this->teamB[i]->i == this->free_ball->i 
                                && this->teamB[i]->j == this->free_ball->j) {
                                playersToFight.push_back(this->teamB[i]);
                            }
                        }
                        if (playersToFight.size() == 1) {
                            playersToFight[0]->takeBall(this->free_ball);
                            this->free_ball = nullptr;
                        } else if(playersToFight.size() == 2) {
                            this->fairFightBall(*playersToFight[0], *playersToFight[1]);
                        }
                    } else if (is_neighbor(this->free_ball->i, this->free_ball->j, this->goalA)
                                && is_neighbor(this->free_ball->i, this->free_ball->j, this->goalB)) {
                        this->free_ball->step_back_one();
                    }
                }
            } else {
                bool alreadyFight = false;
                for (int i = 0; i < 3; i++) {
                    if (alreadyFight) {
                        break;
                    }
                    if (this->teamA[i]->ball != nullptr) {
                        for (int j = 0; j < 3; j++) {
                            if (this->teamA[i]->i == this->teamB[j]->i
                                && this->teamA[i]->j == this->teamB[j]->j) {
                                this->fightBall(*(this->teamA[i]), *(this->teamB[j]));
                                alreadyFight = true;
                                break;
                            }
                        }
                    }
                }
                if (!alreadyFight) {
                    for (int i = 0; i < 3; i++) {
                        if (alreadyFight) {
                            break;
                        }
                        if (this->teamB[i]->ball != nullptr) {
                            for (int j = 0; j < 3; j++) {
                                if (this->teamB[i]->i == this->teamA[j]->i
                                    && this->teamB[i]->j == this->teamA[j]->j) {
                                    this->fightBall(*(this->teamB[i]), *(this->teamA[j]));
                                    alreadyFight = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            this->updateNewState();
            return this->updateScore();
        }

        void undoMove() {
            // Al parecer solo sirve para retroceder UN movimiento

            if (this->last_state == nullptr) {
                return;
            }

            // Busco la pelota
            Ball* ball = this->free_ball;
            for (auto p : this->teamA) {
                if (p->ball != nullptr) ball = p->ball;
            }
            for (auto p : this->teamB) {
                if (p->ball != nullptr) ball = p->ball;
            }
            
            // Pongo posiciones anteriores para jugadores A
            for (auto p_status : this->last_state->team) {
                for (auto p : teamA) {
                    if (p_status.id == p->id) {
                        p->i = p_status.i;
                        p->j = p_status.j;
                        if (p_status.in_posetion) {
                            p->ball = ball;
                        }
                    }
                }
            }

            // Pongo posiciones anteriores para jugadores B
            for (auto p_status : this->last_state->oponent_team) {
                for (auto p : teamB) {
                    if (p_status.id == p->id) {
                        p->i = p_status.i;
                        p->j = p_status.j;
                        if (p_status.in_posetion) {
                            p->ball = ball;
                        }
                    }
                }
            }

            // Si la pelota estaba libre
            this->free_ball = nullptr;
            if (this->last_state->ball.is_free) {
                this->free_ball = ball;
            }

            ball->i = this->last_state->ball.i;
            ball->j = this->last_state->ball.j;
            int dir = this->last_state->ball.dir;
            int steps = this->last_state->ball.steps;
            ball->movement = make_tuple(dir, steps);
        }

        string winner() {
            if (scoreA > scoreB) {
                return "A";
            } else if (scoreB > scoreA) {
                return "B";
            } else {
                return "EMPATARON";
            }
        }

        vector < Player* > updateScore() {
            Ball * ball = this->free_ball;

            if (ball == nullptr) {
                for (auto p: this->teamA) {
                    if (p->ball != nullptr) {
                        ball = p->ball;
                        p->ball = nullptr;
                        this->free_ball = ball;
                    }
                }

                for (auto p: this->teamB) {
                    if (p->ball != nullptr) {
                        ball = p->ball;
                        p->ball = nullptr;
                        this->free_ball = ball;
                    }
                }
            }

            // Si la pelota está en el arco de A, le suma un gol a B y devuelve "A"
            for (auto g: this->goalA) {
                if (get < 0 > (g) == ball->i && get < 0 > (g) == ball->j) {
                    this->scoreB++;
                    return this->teamA;
                }
            }

            // Si la pelota está en el arco de B, le suma un gol a A y devuelve "B"
            for (auto g: this->goalB) {
                if (get < 0 > (g) == ball->i && get < 0 > (g) == ball->j) {
                    this->scoreA++;
                    return this->teamB;
                }
            }
            
            vector < Player* > empty;
            return empty;
        }

        bool positionInBoard(int i, int j) {
            return 0 <= i && i < rows && 0 <= j && j < columns;
        }

        void reset(vector< player_status > positionsA, vector< player_status > positionsB, bool isStartingA) {
            this->startingPositions(positionsA, positionsB, isStartingA);
            this->scoreA = 0;
            this->scoreB = 0;
        }

        void startingPositions(vector< player_status > positionsA, vector< player_status > positionsB, bool isStartingA) {
            // Saco la pelota del juego
            for (auto p: this->teamA) {
                p->ball = nullptr;
            }
            for (auto p: this->teamB) {
                p->ball = nullptr;
            }

            this->free_ball = nullptr;

            // Coloco los jugadores en las posiciones correctas
            for (int i = 0; i < 3;  i++) {
                this->teamA[positionsA[i].id]->i = positionsA[i].i;
                this->teamA[positionsA[i].id]->j = positionsA[i].j;
            }
            for (int i = 0; i < 3;  i++) {
                this->teamB[positionsB[i].id]->i = positionsB[i].i;
                this->teamB[positionsB[i].id]->j = positionsB[i].j;
            }

            // Le doy la pelota al jugador que saca y lo pongo en el centro
            if (isStartingA) {                
                this->teamA[0]->i = int(this->rows / 2);
                this->teamA[0]->j = (this->columns / 2) - 1;
                this->teamA[0]->takeBall(new Ball());
            } else {
                this->teamB[0]->i = int(this->rows / 2);
                this->teamB[0]->j = (this->columns / 2) - 1;
                this->teamB[0]->takeBall(new Ball());
            }
        }
            
        void getState() {
            if (this->last_state == nullptr) {
                this->last_state = new board_status();
            }
            this->last_state->clear();

            Ball* ball = nullptr;

            if (this->free_ball != nullptr) {
                ball = this->free_ball;
                this->last_state->ball.is_free = true;
                this->last_state->ball.i = ball->i;
                this->last_state->ball.j = ball->j;
                this->last_state->ball.dir = get<0>(ball->movement);
                this->last_state->ball.dir = get<1>(ball->movement);
            }

            for (auto p : this->teamA) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    this->last_state->ball.is_free = false;
                }
                this->last_state->team.push_back(player_status(id,i,j,in_posetion));
            }

            for (auto p : this->teamB) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    this->last_state->ball.is_free = false;
                }
                this->last_state->oponent_team.push_back(player_status(id,i,j,in_posetion));
            }
        }

        void updateNewState() {
            if (this->new_state == nullptr) {
                this->new_state = new board_status();
            }
            this->new_state->clear();

            Ball* ball = nullptr;

            if (this->free_ball != nullptr) {
                ball = this->free_ball;
                this->new_state->ball.is_free = true;
                this->new_state->ball.i = ball->i;
                this->new_state->ball.j = ball->j;
                this->new_state->ball.dir = get<0>(ball->movement);
                this->new_state->ball.dir = get<1>(ball->movement);
            }

            for (auto p : this->teamA) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    this->new_state->ball.is_free = false;
                }
                this->new_state->team.push_back(player_status(id,i,j,in_posetion));
            }

            for (auto p : this->teamB) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    this->new_state->ball.is_free = false;
                }
                this->new_state->oponent_team.push_back(player_status(id,i,j,in_posetion));
            }
        }

        board_status getNewState() {
            return *(this->new_state);
        }

        vector < tuple < int, int > > getGoal(const string & team) {
            vector < tuple < int, int > > goal;
            if (team == "A") {
                goal = goalA;
            } else if (team == "B") {
                goal = goalB;
            }
            return goal;
        }

        void print() {
            for (auto p : this->teamA) {
                p->printPlayer();
            }
            for (auto p : this->teamB) {
                p->printPlayer();
            }
            if (this->free_ball != nullptr) {
                cout << this->free_ball->i << " " << this->free_ball->j << endl;
            }
        }

        vector < tuple < int, int > > CreateGoal(int col) {
            vector < tuple < int, int > > goal;
            int floor = (this->rows / 2) - 1;

            for (int i = 0; i < 3; ++i) {
                goal.push_back(make_tuple(floor + i, col));
            }

            return goal;
        }

};

//// Para testear copypaste cambiar f <<
// std::string fileName = "logiboard.log";
// std::ofstream f;
// f.open(fileName, std::fstream::out);
// f <<  " chau " << player->i << " " << player->j << std::endl;
// f.close(); 