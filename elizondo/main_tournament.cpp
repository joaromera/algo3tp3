#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 10;
    Tournament tournament = Tournament(candidates);
    tournament.generate_random_combinations(candidates);
    tournament.print_combinations();
    tournament.play_tournament();
    cout << endl;
    tournament.print_winner();
    cout << endl;
    tournament.print_score_table();
    // cout << "lcoal search" << endl;
    // tournament.local_search(tournament.get_winner(), 0.1);
    // tournament.check_comb();
    
    cout << "hill climb" << endl;
    auto start = chrono::steady_clock::now();
    tournament.hill_climbing(tournament.get_winner(), 0.1);
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << "Tiempo utilizado: " << chrono::duration <double, milli> (diff).count() << " ms." << endl;
    tournament.print_winner();

    // vector < double > test = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
    // tournament.local_search(test, 0.25);
    // tournament.print_combinations();
    // tournament.check_comb();
    return 0;
}