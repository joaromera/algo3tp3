#include "logical_board.cpp"
#include "board_status.hpp"

using namespace std;

void Test_team_movements();

int main() {
    Test_team_movements();

    return 0;
}

void Test_team_movements() {
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
        {2, "MOVIMIENTO", 8}
    };
    
    for (int i = 0; i < 3; i++) {
        board.teamA[i]->i = i;
        board.teamA[i]->j = 0;
    }

    for (int i = 0; i < 3; i++) {
        board.teamB[i]->i = i;
        board.teamB[i]->j = m-1;
    }

    board.makeMove(moves_A, moves_B);
    
    board_status* updated = board.getCurrentState();

    if (board.teamA[0]->i != 1 && board.teamA[0]->j != 0) {
        cout << "jugador 0 del equipo A no esta en su posicion" << endl;
    }
    if (board.teamA[1]->i != 1 && board.teamA[1]->j != 1) {
        cout << "jugador 1 del equipo A no esta en su posicion" << endl;
    }
    if (board.teamA[2]->i != 3 && board.teamA[2]->j != 1) {
        cout << "jugador 2 del equipo A no esta en su posicion" << endl;
    }    
    if (board.teamB[0]->i != 0 && board.teamB[0]->j != m-1) {
        cout << "jugador 0 del equipo B no esta en su posicion" << endl;
    }
    if (board.teamB[1]->i != 0 && board.teamB[1]->j != m-2) {
        cout << "jugador 1 del equipo B no esta en su posicion" << endl;
    }
    if (board.teamB[2]->i != 2 && board.teamB[2]->j != m-2) {
        cout << "jugador 2 del equipo B no esta en su posicion" << endl;
    }
    
    for (int i = 0; i < 3; ++i)
    {
        if (board.teamA[i]->i != updated->team[i].i || board.teamA[i]->j != updated->team[i].j) {
            cout << "el status board no refleja la posicion del jugador " << i << " de mi equipo" << endl;  
        }            
        if (board.teamB[i]->i != updated->oponent_team[i].i || board.teamB[i]->j != updated->oponent_team[i].j) {
            cout << "el status board no refleja la posicion del jugador " << i << " de mi equipo oponente" << endl;
        }
    }
}