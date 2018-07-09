#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"
#include "auxiliars.hpp"
#include "referee.cpp"

using namespace std;

class Tournament {
    public:
        vector < vector < double > > combinations;
        vector < double > current_winner;
        vector < int > scores;
        vector < vector < bool > > already_played;
        int weights_amount = 10;

        Tournament(int candidates) {

            vector < vector < bool > > ap(candidates, vector < bool > (candidates, false));
            this->already_played = ap;

            vector < int > sc(candidates, 0);
            this->scores = sc;
        }

        // limpia los vectors combinations, scores y already_played e inicializa los 2 últimos en 0.
        void reset(int candidates) {
            this->combinations.clear();

            vector < vector < bool > > ap(candidates, vector < bool > (candidates, false));
            this->already_played = ap;

            vector < int > sc(candidates, 0);
            this->scores = sc;
        }

        // llama a reset y llena this->combinations con vectores de pesos random, para hacer GRASP
        void generate_random_combinations(int candidates) {
            this->reset(candidates);
            for (int i = 0; i < candidates; i++) {
                vector < double > combination(this->weights_amount, 0);
                for (int j = 0; j < combination.size(); j++) {
                    combination[j] = rand() % 101 / (double) 100;
                }
                this->combinations.push_back(combination);
            }
        }

        // devuelve una copia del vector que más puntos hizo en scores
        vector < double > get_winner() {
            auto it = max_element(this->scores.begin(), this->scores.end());
            auto index = it - this->scores.begin();
            return this->combinations[index];
        }

        // hace competir todas las combinaciones entre sí
        void play_tournament() {
            for (int i = 0; i < this->combinations.size(); i++) {
                for (int j = 0; j < this->combinations.size(); j++) {
                    if (i != j && !already_played[i][j]) {
                        vector <player> teamA;
                        for (int l = 0; l < 3; l++) {
                            player aux = player(l, 0.5);
                            teamA.push_back(aux);
                        }
                        vector <player> teamB;
                        for (int l = 0; l < 3; l++) {
                            player aux = player(l, 0.5);
                            teamB.push_back(aux);
                        }

                        //Make teams play
                        // vector < double > test = {1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 0.5}; //esta funcionaba en greedy
                        Referee referee = Referee(10, 5, 125, teamA, teamB, combinations[i], combinations[j]);
                        string winner = referee.runPlay(IZQUIERDA);

                        if (winner == IZQUIERDA) {
                            this->scores[i] += 3;
                        } else if (winner == DERECHA) {
                            this->scores[j] += 3;
                        } else {
                            this->scores[i] += 1;
                            this->scores[j] += 1;
                        }
                        this->already_played[i][j] = true;
                    }
                }
            }
        }

        void play_leg(vector < pair < vector < double >, int > > combs) {
            cout << "NOW PLAYING LEG OF " << combs.size() << endl;
            vector < pair < vector < double >, int > > winners;
            for (int i = 0; i < combs.size(); i += 2) {
                vector <player> teamA;
                for (int l = 0; l < 3; l++) {
                    player aux = player(l, 0.5);
                    teamA.push_back(aux);
                }
                vector <player> teamB;
                for (int l = 0; l < 3; l++) {
                    player aux = player(l, 0.5);
                    teamB.push_back(aux);
                }

                //Make teams play
                // vector < double > test = {1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 0.5}; //esta funcionaba en greedy
                Referee referee = Referee(10, 5, 125, teamA, teamB, combs[i].first, combs[i+1].first);
                string winner = referee.runPlay(IZQUIERDA);

                if (winner == IZQUIERDA) {
                    this->scores[combs[i].second] += 3;
                    winners.push_back(combs[i]);
                } else if (winner == DERECHA) {
                    this->scores[combs[i + 1].second] += 3;
                    winners.push_back(combs[i + 1]);
                } else {
                    this->scores[combs[i].second] += 1;
                    this->scores[combs[i + 1].second] += 1;
                    winners.push_back(combs[i]);
                }                
            }
            if (winners.size() > 1) {
                play_leg(winners);
            }
        }

        void elimination_cup(vector < vector < double > > combs) {
            vector < pair < vector < double >, int > > indexed_combs;
            indexed_combs.reserve(combs.size());
            for (int i = 0; i < combs.size(); i++) {
                indexed_combs.push_back(make_pair(combs[i],i));
            }

            play_leg(indexed_combs);
        }

        // genera los vecinos del input, juega el torneo, guarda el ganador
        // genera los vecinos del ganador, juega el torneo, si es mejor lo reemplaza
        // si no es mejor que el ganador anterior, termina y devuelve el ganador anterior
        vector < double > hill_climbing(vector < double > vec, double distance) {
            this->local_search(vec, distance);
            // this->play_tournament();
            this->elimination_cup(this->combinations);
            vector < double > winner = this->get_winner();
            vector < double > old_winner;
            int index = 0;
            do {
                cout << "Vecindario número: " << index << endl;
                index++;
                old_winner = winner;
                this->local_search(old_winner, distance);
                // this->play_tournament();
                this->elimination_cup(this->combinations);
                winner = this->get_winner();
            } while (winner != old_winner);

            return winner;
        }

        vector < double > grasp(double distance) {
            this->generate_random_combinations(10);
            this->play_tournament();

            vector < double > random_solution = this->get_winner();
            this->local_search(random_solution, distance);
            this->play_tournament();

            vector < double > winner = this->get_winner();
            vector < double > old_winner;

            do {
                old_winner = winner;

                this->generate_random_combinations(10);
                this->play_tournament();
                random_solution = this->get_winner();

                this->local_search(random_solution, distance);
                this->combinations.push_back(old_winner);
                this->play_tournament();
                winner = this->get_winner();
                
            } while (winner != old_winner);

            return winner;
        }

        // helper de local_search
        void local_search_recursive(vector < double > vec, int index, double distance) {
            if (index < vec.size()) {
                vector < double > mod_vec = vec;
                mod_vec[index] += distance;
                if (mod_vec[index] > 1) mod_vec[index] = 1;
                if (mod_vec[index] < 0) mod_vec[index] = 0;

                // reemplazar estas tres de abajo por las otras cuatro comentadas
                vec[index] -= distance;
                if (vec[index] > 1) vec[index] = 1;
                if (vec[index] < 0) vec[index] = 0;
                // vector < double > mod_vec2 = vec;
                // mod_vec2[index] -= distance;
                // if (mod_vec2[index] > 1) mod_vec2[index] = 1;
                // if (mod_vec2[index] < 0) mod_vec2[index] = 0;

                index++;
                local_search_recursive(vec, index, distance);
                local_search_recursive(mod_vec, index, distance);
                // local_search_recursive(mod_vec2, index, distance); // si se hizo lo de arriba, descomentar esta linea y la ultima
                this->combinations.push_back(mod_vec);
                // this->combinations.push_back(mod_vec2);
            }
        }

        // llama a reset y sobrescribe this->combinations con los vecinos de vec
        void local_search(vector < double > vec, double distance) {
            int size = pow(2, vec.size());
            this->reset(size);

            this->local_search_recursive(vec, 0, distance);
            this->combinations.push_back(vec);
        }

        // imprime todos los vectores en combinations
        void print_combinations() {
            for (auto combination: this->combinations) {
                for (int i = 0; i < combination.size(); i++) {
                    cout << combination[i] << " ";
                }
                cout << endl;
            }
        }

        void print_score_table() {
            for (int i = 0; i < this->combinations.size(); i++) {
                cout << "COMBINATION: ";
                for (int j = 0; j < combinations[i].size(); j++) {
                    cout << combinations[i][j] << " ";
                }
                cout << "SCORE: " << this->scores[i] << endl;
            }
        }

        // imprime el vector con más puntos según score
        void print_winner() {
            auto it = max_element(this->scores.begin(), this->scores.end());
            auto index = it - this->scores.begin();
            for (int i = 0; i < this->combinations[0].size(); i++) {
                cout << combinations[index][i] << " ";
            }
            cout << endl;
        }

        // estoy chequeando que todas las combinaciones sean distintas entre sí
        void check_comb() {
            cout << this->combinations.size() << endl;
            for (int i = 0; i < this->combinations.size(); i++) {
                for (int j = 0; j < this->combinations.size(); j++) {
                    if (i != j) {
                        if (this->combinations[i] == this->combinations[j]) {
                            cout << "IGUALES" << endl;
                        }
                    }
                }
            }
        }
};