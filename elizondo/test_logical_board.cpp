#include "logical_board.cpp"

using namespace std;

int main() {
    int n, m, s;
    std::string lado;
    cin >> n >> m >> s >> lado;
    
    vector <player> teamA;
    for (int i = 0; i < 3; i++) {
        int id;
        double probability;
        cin >> id >> probability;
        teamA.push_back(player(id, probability));
    }

    vector <player> teamB;
    for (int i = 0; i < 3; i++) {
        int id;
        double probability;
        cin >> id >> probability;
        teamB.push_back(player(id, probability));
    }

    LogicalBoard board = LogicalBoard(n,m,teamA, teamB);

    // if (lado == "DERECHA") {
    //     board = LogicalBoard(n,m,teamA, teamB);
        
    // } else {
    //     board = LogicalBoard(n,m,teamB, teamA);
    // }
    
    return 0;
}