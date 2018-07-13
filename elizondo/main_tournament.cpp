#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "tournament.cpp"

int main(int argc, char **argv) {
    Tournament tournament = Tournament(64);
    tournament.genetic(64, true, true, false);
    tournament.print_score_table();
    return 0;
}