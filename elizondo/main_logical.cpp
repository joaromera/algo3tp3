#include <assert.h>

#include "logical_board.cpp"
#include "board_status.hpp"

using namespace std;

void test_movements();

int main() {
    test_movements();
    return 0;
}

void test_movements() {
    int n = 10;
    int m = 5;
    
    vector <player> teamA;
    for (int i = 0; i < 3; i++) {
        player p = player(i, 0.5);
        teamA.push_back(p);
    }

    vector <player> teamB;
    for (int i = 0; i < 3; i++) {
        player p = player(i, 0.5);
        teamB.push_back(p);
    }

    LogicalBoard board = LogicalBoard(n, m, teamA, teamB);

    vector<player_move> moves_A = {
        {0, "MOVIMIENTO", 6},
        {1, "MOVIMIENTO", 4},
        {2, "MOVIMIENTO", 5}
    };

    vector<player_move> moves_B = {
        {0, "MOVIMIENTO", 0},
        {1, "MOVIMIENTO", 7},
        {2, "MOVIMIENTO", 7}
    };
    
    for (int i = 0; i < 3; i++) {
        board.teamA[i]->i = i;
        board.teamA[i]->j = 0;
    }

    for (int i = 0; i < 3; i++) {
        board.teamB[i]->i = i;
        board.teamB[i]->j = m-1;
    }

    // Jugador 0 del equipo A está en la posición (0,0)
    assert(board.teamA[0]->i == 0 && board.teamA[0]->j == 0);

    // Jugador 1 del equipo A se movió a la posición (1,0)
    assert(board.teamA[1]->i == 1 && board.teamA[1]->j == 0);

    // Jugador 2 del equipo A se movió a la posición (2,0)
    assert(board.teamA[2]->i == 2 && board.teamA[2]->j == 0);

    // Jugador 0 del equipo B se movió a la posición (0,4)
    assert(board.teamB[0]->i == 0 && board.teamB[0]->j == 4);

    // Jugador 1 del equipo B se movió a la posición (1,4)
    assert(board.teamB[1]->i == 1 && board.teamB[1]->j == 4);

    // Jugador 2 del equipo B se movió a la posición (2,4)
    assert(board.teamB[2]->i == 2 && board.teamB[2]->j == 4);
    
    board.makeMove(moves_A, moves_B);
    board_status* updated = board.getCurrentState();
    
    // Jugador 0 del equipo A se movió a la posición (1,0)
    assert(updated->team[0].i == 1 && updated->team[0].j == 0);

    // Jugador 1 del equipo A se movió a la posición (1,1)
    assert(updated->team[1].i == 1 && updated->team[1].j == 1);

    // Jugador 2 del equipo A se movió a la posición (3,1)
    assert(updated->team[2].i == 3 && updated->team[2].j == 1);

    // Jugador 0 del equipo B se movió a la posición (0,4)
    assert(updated->oponent_team[0].i == 0 && updated->oponent_team[0].j == m-1);

    // Jugador 1 del equipo B se movió a la posición (2,3)
    assert(updated->oponent_team[1].i == 2 && updated->oponent_team[1].j == m-2);

    // Jugador 2 del equipo B se movió a la posición (3,3)
    assert(updated->oponent_team[2].i == 3 && updated->oponent_team[2].j == m-2);
}