#include <vector>
#include <random>
#include <iostream>

using namespace std;

class Tournament {
    public:
    vector < vector < double > > combinations;
    vector < int > scores;
    vector < vector < bool > > already_played;
    int weights_amount = 10;

    Tournament (int participants) {

        vector < vector < bool > > ap (participants, vector < bool > (participants, false));
        this-> already_played = ap;

        vector < int > sc (participants, 0);
        this->scores = sc;
    }

    void generate_random_combinations(int participants) {
        this->combinations.clear();
        for (int i = 0; i < participants; i++) {
            vector < double > team (this->weights_amount,0);
            for (int j = 0; j < team.size(); j++) {
                team[j] = rand() % 101 / (double) 100;
            }
            this->combinations.push_back(team);
        }

        vector < vector < bool > > ap (participants, vector < bool > (participants, false));
        this->already_played = ap;

        vector < int > sc (participants, 0);
        this->scores = sc;
    }

    vector < double > get_leader() {
        auto it = max_element(this->scores.begin(), this->scores.end());
        auto index = it - this->scores.begin();
        return this->combinations[index];
    }

    void play_tournament() {
        for (int i = 0; i < this->combinations.size(); i++) {
            for (int j = 0; j < this->combinations.size(); j++) {
                if (i != j && !already_played[i][j]) {
                    int wins_i = 0;
                    int wins_j = 0;
                    for (int k = 0; k < 5; k++) {
                        wins_i++;
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
    void local_search_recursive(vector < double > vec, int index, double distance) {        
        if (index < vec.size()) {
            vector <double> mod_vec = vec;
            mod_vec[index] += distance;
            index++;    
            local_search_recursive(vec, index, distance);
            local_search_recursive(mod_vec, index, distance);
            this->combinations.push_back(mod_vec);
        }
    }

    // overwrites this->teams
    void local_search(vector < double > vec, double distance) {
        
        this->combinations.clear();
        int size = pow(2,vec.size());

        vector < vector < bool > > ap (size, vector < bool > (size, false));
        this->already_played = ap;

        vector < int > sc (size, 0);
        this->scores = sc;

        this->local_search_recursive(vec, 0, distance);
        this->combinations.push_back(vec);
    }
    
    void print_combinations() {
        for (auto team : this->combinations) {
            for (int i = 0; i < team.size(); i++) {
                cout << team[i] << " ";
            }
            cout << endl;
        }
    }

    void print_leader() {
        auto it = max_element(this->scores.begin(), this->scores.end());
        auto index = it - this->scores.begin();
        for (int i = 0; i < this->combinations[0].size(); i++) {
                cout << combinations[index][i] << " ";
            }
        cout << endl;
    }
};

int main() {
    return 0;
}