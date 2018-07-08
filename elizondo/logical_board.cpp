#ifndef LOGICAL_BOARD
#define LOGICAL_BOARD

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

        LogicalBoard(
            int columns,
            int rows,
            vector < player > teamA,
            vector < player > teamB
        ) {
            this->scoreA = 0;
            this->scoreB = 0;

            vector < Player* > _teamA;
            for(auto p : teamA) {
                Player *aux = new Player(p);
                _teamA.push_back(aux);
            }
            this->teamA = _teamA;

            vector < Player* > _teamB;
            for(auto p : teamB) {
                Player *aux = new Player(p);
                _teamB.push_back(aux);
            }
            this->teamB = _teamB;

            this->columns = columns;
            this->rows = rows;

            this->goalA = CreateGoal(-1);
            this->goalB = CreateGoal(this->columns);

            this->free_ball = new Ball();
            this->last_state = nullptr;
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

            vector < Player* > _teamA;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(i, teamA[i].probability);
                _teamA.push_back(aux);
            }
            this->teamA = _teamA;

            vector < Player* > _teamB;
            for(int i = 0; i < 3; i++) {
                Player *aux = new Player(i, teamB[i].probability);
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
            for (auto aPlayer : *team) {
                for (auto aMove : moves) {
                    if (aMove.player_id == aPlayer->id) {
                        if (aMove.move_type == "MOVIMIENTO") {
                            aPlayer->move(aMove.dir);
                        }
                        if (aMove.move_type == "PASE") {
                            this->free_ball = aPlayer->ball;
                            this->free_ball->setMovement(aMove.dir, aMove.steps);
                            aPlayer->ball = nullptr;
                        }
                    }
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
            
            vector<player_status> team;
            if (isOponent) {
                team = this->last_state->oponent_team;
            } else {
                team = this->last_state->team;
            }

            player_status prevStatePlayer;
            for (auto p : team) {
                if (player->id == p.id) {
                    prevStatePlayer = p;
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

        string makeMove(vector< player_move > movesA, vector< player_move > movesB) {
            this->getState();
            this->makeTeamMove(&(this->teamA), movesA);
            this->makeTeamMove(&(this->teamB), movesB);
            
            if (this->free_ball != nullptr) {
                
                vector< Player* > intercepters;
                for (auto p : this->teamA) {
                    if (this->intercepted(p, false)) {
                        intercepters.push_back(p);
                    }
                }
                for (auto p : this->teamB) {
                    if (this->intercepted(p, true)) {
                        intercepters.push_back(p);
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
                        for (auto p : this->teamA) {
                            if (p->i == this->free_ball->i && p->j == this->free_ball->j) {
                                playersToFight.push_back(p);
                            }
                        }
                        for (auto p : this->teamB) {
                            if (p->i == this->free_ball->i && p->j == this->free_ball->j) {
                                playersToFight.push_back(p);
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
                for (auto playerA : this->teamA) {
                    if (alreadyFight) {
                        break;
                    }
                    if (playerA->ball != nullptr) {
                        for (auto playerB : this->teamB) {
                            if (playerA->i == playerB->i && playerA->j == playerB->j) {
                                this->fightBall(*(playerA), *(playerB));
                                alreadyFight = true;
                                break;
                            }
                        }
                    }
                }
                if (!alreadyFight) {
                    for (auto playerB : this->teamB) {
                        if (alreadyFight) {
                            break;
                        }
                        if (playerB->ball != nullptr) {
                            for (auto playerA : this->teamA) {
                                if (playerB->i == playerA->i && playerB->j == playerA->j) {
                                    this->fightBall(*(playerB), *(playerA));
                                    alreadyFight = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
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
                return IZQUIERDA;
            } else if (scoreB > scoreA) {
                return DERECHA;
            } else {
                return TIE;
            }
        }

        string updateScore() {
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
                    return IZQUIERDA;
                }
            }

            // Si la pelota está en el arco de B, le suma un gol a A y devuelve "B"
            for (auto g: this->goalB) {
                if (get < 0 > (g) == ball->i && get < 0 > (g) == ball->j) {
                    this->scoreA++;
                    return DERECHA;
                }
            }
            
            return TIE;
        }

        bool positionInBoard(int i, int j) {
            return 0 <= i && i < rows && 0 <= j && j < columns;
        }

        void reset(vector< player_status > positionsA, vector< player_status > positionsB, string starting) {
            this->startingPositions(positionsA, positionsB, starting);
            this->scoreA = 0;
            this->scoreB = 0;
        }

        void startingPositions(vector< player_status > positionsA, vector< player_status > positionsB, string starting) {
            // Saco la pelota del juego
            for (auto p: this->teamA) {
                p->ball = nullptr;
            }
            for (auto p: this->teamB) {
                p->ball = nullptr;
            }

            this->free_ball = nullptr;

            // Coloco los jugadores en las posiciones correctas
            for (auto p: positionsA) {
                this->teamA[p.id]->i = p.i;
                this->teamA[p.id]->j = p.j;
            }
            for (auto p: positionsB) {
                this->teamB[p.id]->i = p.i;
                this->teamB[p.id]->j = p.j;
            }

            // Le doy la pelota al jugador que saca y lo pongo en el centro
            if (starting == IZQUIERDA) {
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
                this->last_state->ball.steps = get<1>(ball->movement);
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

        board_status* getCurrentState() {
            board_status* current_state = new board_status();
            current_state->clear();
            Ball* ball = nullptr;

            if (this->free_ball != nullptr) {
                ball = this->free_ball;
                current_state->ball.is_free = true;
                current_state->ball.i = ball->i;
                current_state->ball.j = ball->j;
                current_state->ball.dir = get<0>(ball->movement);
                current_state->ball.steps = get<1>(ball->movement);
            }

            for (auto p : this->teamA) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    current_state->ball.is_free = false;
                }
                current_state->team.push_back(player_status(id,i,j,in_posetion));
            }

            for (auto p : this->teamB) {
                int id = p->id;
                int i = p->i;
                int j = p->j;
                bool in_posetion = false;
                if (p->ball != nullptr) {
                    in_posetion = true;
                    ball = p->ball;
                    current_state->ball.is_free = false;
                }
                current_state->oponent_team.push_back(player_status(id,i,j,in_posetion));
            }
            
            return current_state;
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

#endif //BOARD_STATUS