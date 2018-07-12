#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 20;
    Tournament tournament = Tournament(candidates);
    vector < double > grasp_winner = tournament.grasp(0.20);
    tournament.print_score_table();
    // tournament.genetic(candidates);
    // tournament.print_combinations();
    // tournament.play_tournament();
    // cout << endl;
    // tournament.print_winner();
    // cout << endl;
    // tournament.print_score_table();
    // cout << "lcoal search" << endl;
    // tournament.local_search(tournament.get_winner(), 0.1);
    // tournament.check_comb();
    
    // vector < double > test = {1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 1.0, 0.8, 0.05, 0.5};
    // cout << "hill climb" << endl;
    // auto start = chrono::steady_clock::now();
    // tournament.hill_climbing(tournament.get_winner(), 0.5);
    // auto end = chrono::steady_clock::now();
    // auto diff = end - start;
    // cout << "Tiempo utilizado: " << chrono::duration <double, milli> (diff).count() << " ms." << endl;
    // tournament.print_winner();

    // vector < double > test = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    // tournament.local_search(test, 0.25);
    // tournament.print_combinations();
    // tournament.check_comb();
    return 0;
}