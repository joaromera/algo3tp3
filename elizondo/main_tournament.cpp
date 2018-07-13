#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 20;
    Tournament tournament = Tournament(candidates);
    vector < double > grasp_winner = tournament.grasp(0.20, true, true);
    tournament.print_score_table();
    return 0;
}