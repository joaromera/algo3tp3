#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <queue>
#include <chrono>
#include <assert.h>

#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"
#include "auxiliars.hpp"
#include "referee.cpp"

using namespace std;

class heavierThan {
    public:
        bool operator()(const pair < vector < double >, int > & a, const pair < vector < double >, int > & b) const {
            return (a.second < b.second);
        }
};

typedef std::priority_queue < pair < vector < double >, int >, vector < pair < vector < double >, int > >  , heavierThan > priorityQueue;

class Tournament {
    public:
        vector < vector < double > > combinations;
        vector < int > scores;
        vector < int > goals;
        vector < vector < bool > > already_played;
        int weights_amount = 10;

        Tournament(int candidates) {
            srand(time(NULL));
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

            vector < int > gs(candidates, 0);
            this->goals = gs;
        }

        void reset_scores() {
            int candidates = this->combinations.size();
            vector < vector < bool > > ap(candidates, vector < bool > (candidates, false));
            this->already_played = ap;

            vector < int > sc(candidates, 0);
            this->scores = sc;

            vector < int > gs(candidates, 0);
            this->goals = gs;
        }

        // llama a reset y llena this->combinations con vectores de pesos random, para hacer GRASP o para inicializar la población del GENETIC
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

        // hace competir todas las combinaciones entre sí, actualiza el puntaje en SCORE y marca los equipos que jugaron en ALREADYPLAYED según índices de COMBINATIONS
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
                        Referee referee = Referee(10, 5, 125, teamA, teamB, combinations[i], combinations[j]);
                        string winner = referee.runPlay(IZQUIERDA);

                        this->goals[i] += referee.getScore(IZQUIERDA);
                        this->goals[j] += referee.getScore(DERECHA);

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

        // LIGA DE ELIMINACIÓN DIRECTA, se eliminan de a dos, por lo que en cada iteración se divide a la mitad la cantidad de equipos. también actualiza SCORES.
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
                Referee referee = Referee(10, 5, 125, teamA, teamB, combs[i].first, combs[i+1].first);
                string winner = referee.runPlay(IZQUIERDA);

                this->goals[i] += referee.getScore(IZQUIERDA);
                this->goals[i+1] += referee.getScore(DERECHA);

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

        // Antes de usar PLAY LEG hay que guardar cada combinación con su índice en SCORES
        void elimination_cup() {
            vector < pair < vector < double >, int > > indexed_combs;
            indexed_combs.reserve(this->combinations.size());
            for (int i = 0; i < this->combinations.size(); i++) {
                indexed_combs.push_back(make_pair(this->combinations[i],i));
            }
            play_leg(indexed_combs);
        }

        // genera los vecinos del input, juega el torneo, guarda el ganador
        // genera los vecinos del ganador, juega el torneo, si es mejor lo reemplaza
        // si no es mejor que el ganador anterior, termina y devuelve el ganador anterior
        vector < double > local_search(vector < double > vec, double distance, bool fast, bool elimination, int amount = 64) {
            if (fast) {
                this->fast_neighbourhood(vec, distance, amount);    
            } else {
                this->neighbourhood(vec, distance);    
            }
            if (elimination) {
                this->elimination_cup();
            } else {
                this->play_tournament();
            }
            
            vector < double > winner = this->get_winner();
            vector < double > old_winner;
            int iterations = 0;
            int iterations_alive = 1;
            do {
                cout << "Vecindario número: " << iterations << endl;
                iterations++;
                old_winner = winner;
                if (fast) {
                    this->fast_neighbourhood(vec, distance, amount);    
                } else {
                    this->neighbourhood(vec, distance);    
                }
                if (elimination) {
                    this->elimination_cup();
                } else {
                    this->play_tournament();
                }
                winner = this->get_winner();
                if (winner == old_winner) {
                    iterations_alive++;
                } else {
                    iterations_alive = 1;
                }
            } while (iterations_alive < 5 && iterations < 5);

            return winner;
        }

        vector < double > single_match(const vector < double > & izq, const vector < double > & der) {
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

                Referee referee = Referee(10, 5, 125, teamA, teamB, izq, der);
                string winner = referee.runPlay(IZQUIERDA);

                if (winner == IZQUIERDA) {
                    return izq;
                } else {
                    return der;
                }          
        }

        vector < double > grasp(double distance, bool fast, bool elimination, int amount = 64) {

            this->generate_random_combinations(1);
            this->local_search(this->combinations[0], distance, fast, elimination);

            vector < double > winner = this->get_winner();
            vector < double > old_winner;

            int iterations = 0;
            int iterations_alive = 1;
            do {
                old_winner = winner;

                this->generate_random_combinations(1);
                this->local_search(this->combinations[0], distance, fast, elimination);

                winner = single_match(old_winner, this->get_winner());
                iterations++;
                if (winner == old_winner) {
                    iterations_alive++;
                } else {
                    iterations_alive = 1;
                }

            } while (iterations_alive < 5 && iterations < 5);

            return winner;
        }

        // helper de neighbourhood
        void neighbourhood_recursive(vector < double > vec, int index, double distance) {
            if (index < vec.size()) {
                vector < double > mod_vec = vec;
                mod_vec[index] += distance;
                if (mod_vec[index] > 1) mod_vec[index] = 1;
                if (mod_vec[index] < 0) mod_vec[index] = 0;

                vec[index] -= distance;
                if (vec[index] > 1) vec[index] = 1;
                if (vec[index] < 0) vec[index] = 0;

                index++;
                neighbourhood_recursive(vec, index, distance);
                neighbourhood_recursive(mod_vec, index, distance);
                this->combinations.push_back(mod_vec);
            }
        }

        // genera vecindario recursivamente. llama a reset y sobrescribe this->combinations con los vecinos de vec. todas las combinaciones sumando y restando distance
        void neighbourhood(vector < double > vec, double distance) {
            int size = pow(2, vec.size());
            this->reset(size);

            this->neighbourhood_recursive(vec, 0, distance);
            this->combinations.push_back(vec);
        }

        // genera 64 vecinos a partir de un vector de entrada, suma o resta distance
        void fast_neighbourhood(vector < double > comb, double distance, int amount) {
            this->reset(amount);
            this->combinations.push_back(comb);
            for (int i = 0; i < amount - 1; i++) {
                vector < double > neighbour(10,0);
                for (int j = 0; j < this->weights_amount; j++) {
                    int chance = rand() % 101;
                    if (chance < 50) {
                        neighbour[j] = comb[j] + distance;
                        if (neighbour[i] > 1) neighbour[j] = 1;
                    } else {
                        neighbour[j] = comb[j] - distance;
                        if (neighbour[i] < 0) neighbour[j] = 0;
                    }
                }
                this->combinations.push_back(neighbour);
            }
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

        // imprime la tabla con los puntos de cada combinacion
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

        vector < double > crossover_half (const vector < double > & parent_A, const vector < double > & parent_B) {
            vector < double > child;
            for (int i = 0; i < 3; i ++) {
                child.push_back(parent_A[i]);
                if (rand() % 101 < 5) {
                    child[i] = rand() % 101 / (double) 100;
                }
            }
            for (int i = 3; i < 6; i ++) {
                child.push_back(parent_B[i]);
                if (rand() % 101 < 5) { // MUTACION
                    child[i] = rand() % 101 / (double) 100;
                }
            }
            for (int i = 6; i < 10; i ++) {
                child.push_back(parent_A[i]);
                if (rand() % 101 < 5) {
                    child[i] = rand() % 101 / (double) 100;
                }
            }
            return child;
        }

        vector < double > crossover_random (const vector < double > & parent_A, const vector < double > & parent_B) {
            vector < double > child;
            for (int i = 0; i < parent_A.size(); i++) {
                if (rand() % 101 < 50) {
                    child.push_back(parent_A[i]);
                } else {
                    child.push_back(parent_B[i]);
                }
                if (rand() % 101 < 5) { // MUTACION
                    child[i] = rand() % 101 / (double) 100;
                }
            }
            return child;
        }

        void crossover(int population, bool deterministic, bool crossover_half) {
            int size = this->combinations.size();
            if (deterministic) {
                for (int i = 0; i < size - 1; i += 2) {
                    if (crossover_half) {
                        this->combinations.push_back(this->crossover_half(this->combinations[i], this->combinations[i + 1]));
                        this->combinations.push_back(this->crossover_half(this->combinations[i + 1], this->combinations[i]));
                    } else {
                        this->combinations.push_back(this->crossover_random(this->combinations[i], this->combinations[i + 1]));
                        this->combinations.push_back(this->crossover_random(this->combinations[i + 1], this->combinations[i]));
                    }
                }
            } else {
                int parent_A = 0;
                int parent_B = 1;
                int parents = size;
                while (size < population) {
                    if (crossover_half) {
                        this->combinations.push_back(this->crossover_half(this->combinations[parent_A], this->combinations[parent_B]));
                        this->combinations.push_back(this->crossover_half(this->combinations[parent_B], this->combinations[parent_A]));
                    } else {
                        this->combinations.push_back(this->crossover_random(this->combinations[parent_A], this->combinations[parent_B]));
                        this->combinations.push_back(this->crossover_random(this->combinations[parent_B], this->combinations[parent_A]));
                    }
                    parent_A = (parent_A + 1) % parents;
                    parent_B = (parent_B + 1) % parents;
                }
            }
        }

        void selection(bool deterministic, bool scores) {
            priorityQueue ranking;
            for (int i = 0; i < this->combinations.size(); i++) {
                if (scores) {
                    ranking.push(make_pair(this->combinations[i], this->scores[i])); // FITNESS POR PUNTOS EN TABLA
                } else {
                    ranking.push(make_pair(this->combinations[i], this->goals[i])); // FITNESS POR CANTIDAD DE GOLES
                }
            }
            if (deterministic) {
                int new_size = this->combinations.size() / 2;
                this->reset(this->combinations.size());
                for (int i = 0; i < new_size; i ++) {
                    this->combinations.push_back(ranking.top().first);
                    ranking.pop();
                }
            } else {
                double max_score = ranking.top().second;
                if (scores) max_score *= 3;
                int new_size = this->combinations.size() / 2;
                this->reset(this->combinations.size());
                while (this->combinations.size() < new_size || ranking.size() == 0) {
                    double probability = ranking.top().second / max_score;
                    if (rand() % 101 < probability) {
                        this->combinations.push_back(ranking.top().first);
                    }
                    ranking.pop();
                }
            }
        }

        void genetic(int population, bool deterministic, bool elimination, bool crossover_half, bool scores, int generations) {
            cout << "IN GENETIC" << endl;
            this->generate_random_combinations(population);
            if (elimination) {
                this->elimination_cup();
            } else {
                this->play_tournament();
            }

            vector < double > winner = this->get_winner();
            vector < double > old_winner;

            int iterations_alive = 1;
            int iterations = 0;
            while (iterations < generations && iterations_alive < 5) {
                old_winner = winner;
                cout << "ITERATIONS: " << iterations << endl;
                this->print_score_table();
                this->selection(deterministic, scores);
                this->crossover(population, deterministic, crossover_half);
                if (elimination) {
                    this->elimination_cup();
                } else {
                    this->play_tournament();
                }
                winner = this->get_winner();
                if (winner == old_winner) {
                    iterations_alive++;
                } else {
                    iterations_alive = 1;
                }
                iterations++;
            }
        }
};