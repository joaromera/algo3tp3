#include <string>
#include <random>
#include <vector>
#include <map>

#include "tournament.cpp"

int main(int argc, char **argv) {
    int candidates = 10;
    Tournament* tournament = new Tournament(candidates);
    tournament->generate_random_combinations(candidates);
    tournament->play_tournament();
    return 0;
}