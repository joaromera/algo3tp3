#include "logical_board.cpp"
#include "board_status.hpp"

using namespace std;

int main() {
    int n = 10;
    int m = 5;
    
    vector <player> teamA;
    for (int i = 0; i < 3; i++) {
        teamA.push_back(player(i, 0.5));
    }

    vector <player> teamB;
    for (int i = 0; i < 3; i++) {
        teamB.push_back(player(i, 0.5));
    }

    LogicalBoard board = LogicalBoard(n, m, teamA, teamB);

    vector<player_move> moves_A = {
        {0, "MOVIMIENTO", 2},
        {1, "MOVIMIENTO", 2},
        {2, "MOVIMIENTO", 2}
    };

    vector<player_move> moves_B = {
        {0, "MOVIMIENTO", 8},
        {1, "MOVIMIENTO", 8},
        {2, "MOVIMIENTO", 8}
    };

    board.makeMove(moves_A, moves_B);

    board_status updated = board.getNewState();

    return 0;
}