#include <vector>
#include <random>
#include <iostream>

using namespace std;

class Tournament {
    vector < vector < double > > teams;
    vector < int > scores;
    vector < vector < bool > > already_played;

    Tournament (int participants) {
        this->teams.reserve(participants);
        this->scores.reserve(participants);
        this->already_played.reserve(participants);

        for (int i = 0; i < participants; i++) {
            this->already_played[i].reserve(participants);
        }
    }

    void generate_random_teams(int participants) {
        this->teams.clear();
        for (int i = 0; i < participants; i++) {
            vector < double > team (10,0);
            for (int j = 0; j < 10; j++) {
                team[j] = rand() % 101 / (double) 100;
                //cout << best[j] << endl;
            }
            this->teams.push_back(team);
        }

        this->scores.reserve(participants);
        this->already_played.reserve(participants);
        for (int i = 0; i < participants; i++) {
            this->already_played[i].reserve(participants);
        }
    }

    vector < double > get_leader() {
        return this->teams[*max(scores.begin(), scores.end())];
    }

    void play_tournament() {
        for (int i = 0; i < this->teams.size(); i++) {
            for (int j = 0; j < this->teams.size(); j++) {
                if (i != j && !already_played[i][j]) {
                    int wins_i = 0;
                    int wins_j = 0;
                    for (int k = 0; k < 5; k++) {

                        // JUEGAN

                        /**
                        if (gano i) {
                            wins_i++;
                        } else if (gano j) {
                            wins_j++;
                        }
                        **/
                    }
                    if (wins_i > wins_j) {
                        this->scores[i] += 3;
                    } else if (wins_i < wins_j) {
                        this->scores[j] += 3;
                    } else {
                        this->scores[i] += 1;
                        this->scores[j] += 1;
                    }
                    this->already_played[i][j] = true;
                    this->already_played[j][i] = true;
                }
            }
        }
    }

    // modifies VECTORS with all neighbours of VEC
    void local_search_recursive(vector < vector < double > > & vectors, vector < double > vec, int index, double distance) {
        if (index < vec.size()) {
            vector <double> mod_vec = vec;
            mod_vec[index] += distance;

            index++;
            local_search_recursive(vectors, vec, index, distance);
            local_search_recursive(vectors, mod_vec, index, distance);
        }
        vectors.push_back(vec);
    }

    // overwrites this->teams
    void local_search(const vector < double > & neighbourhood) {
        this->teams.clear();
        for (double a = 0; a <= 0.5; a += 0.5) { // en vez de hacer 10 for anidados se puede hacer recursivamente pero que lo piense otro
            for (double b = 0; b <= 0.5; b += 0.5) {
                for (double c = 0; c <= 0.5; c += 0.5) {
                    for (double d = 0; d <= 0.5; d += 0.5) {
                        for (double e = 0; e <= 0.5; e += 0.5) {
                            for (double f = 0; f <= 0.5; f += 0.5) {
                                for (double g = 0; g <= 0.5; g += 0.5) {
                                    for (double h = 0; h <= 0.5; h += 0.5) {
                                        for (double i =0; i <= 0.5; i += 0.5) {
                                            for (double j = 0; j <= 0.5; j += 0.5) {
                                                vector < double > neighbour = {
                                                    neighbourhood[0] + a,
                                                    neighbourhood[1] + b,
                                                    neighbourhood[2] + c,
                                                    neighbourhood[3] + d,
                                                    neighbourhood[4] + e,
                                                    neighbourhood[5] + f,
                                                    neighbourhood[6] + g,
                                                    neighbourhood[7] + h,
                                                    neighbourhood[8] + i,
                                                    neighbourhood[9] + j
                                                };
                                                this->teams.push_back(neighbour);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

};

int main() {
}