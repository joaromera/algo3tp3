#include <string>
#include <random>
#include <vector>
#include <map>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 10;
    Tournament* tournament = new Tournament(candidates);
    tournament->generate_random_combinations(candidates);
    // tournament->play_tournament();
    tournament->print_winner();
    tournament->print_combinations();
    vector < double > winner = tournament->get_winner();
    cout << "reset"<< endl;
    tournament->reset(5);
    tournament->print_combinations();
    cout << "NEW COMB"<<endl;
    tournament->generate_random_combinations(5);
    tournament->print_combinations();
    cout << "NEW COMB" << endl;
    cout << "********" << endl;
    tournament->print_winner();
    cout << "********" << endl;
    winner = tournament->get_winner();
    tournament->local_search(winner,0.10);
    tournament->print_combinations();
    tournament->check_comb();
    return 0;
}