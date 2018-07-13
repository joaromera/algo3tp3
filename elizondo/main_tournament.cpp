#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 32;
    Tournament tournament = Tournament(candidates);
    tournament.genetic(candidates, true, true, true);
    tournament.print_score_table();
    return 0;
}