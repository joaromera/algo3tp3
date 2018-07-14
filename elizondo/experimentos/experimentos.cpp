#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "../tournament.cpp"

void test_time_allvsall();
void test_time_elimination();
void test_quality_allvsall_elimination_with_same_teams();
void test_quality_allvsall_elimination_same_time();

int main(int argc, char **argv) {
    
    // test_time_allvsall();
    // test_time_elimination();
    // test_quality_allvsall_elimination_with_same_teams();
    test_quality_allvsall_elimination_same_time();

    return 0;
}

void test_time_allvsall() {
    string fileName = "test_time_allvsall.txt";
    ofstream results;
    results.open(fileName, fstream::out);

    Tournament tournament = Tournament(2);
    for (int i = 2; i <= 32; i++) {
        tournament.generate_random_combinations(i);
        chrono::duration < double, milli > average = (chrono::duration < double, milli > ) 0;
        for (int j = 0; j < 5; j++) {
            auto start = chrono::steady_clock::now();
            tournament.play_tournament();
            auto end = chrono::steady_clock::now();
            average += end - start;
            tournament.print_score_table();
        }
        results << "all vs all;" << i << ";" << chrono::duration < double, milli > (average / 5).count() << ";" << endl;
    }
    results.close();
}

void test_time_elimination() {
    string fileName = "test_time_elimination.txt";
    ofstream results;
    results.open(fileName, fstream::out);

    Tournament tournament = Tournament(2);
    for (int i = 2; i < 2048; i *= 2) {
        tournament.generate_random_combinations(i);
        chrono::duration < double, milli > average = (chrono::duration < double, milli > ) 0;
        for (int j = 0; j < 5; j++) {
            auto start = chrono::steady_clock::now();
            tournament.elimination_cup();
            auto end = chrono::steady_clock::now();
            average += end - start;
            tournament.print_score_table();
        }
        results << "elimination;" << i << ";" << chrono::duration < double, milli > (average / 5).count() << ";" << endl;
    }
    results.close();
}

void test_quality_allvsall_elimination_with_same_teams() {
    string fileName = "test_quality_allvsall_elimination_with_same_teams.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 2; i <= 32; i *= 2) {

        Tournament tour = Tournament(i);
        tour.generate_random_combinations(i);

        tour.play_tournament();
        cout << "Ganador todos contra todos" << endl;
        tour.print_winner();
        vector < double > all = tour.get_winner();
        
        tour.reset_scores();
        tour.elimination_cup();
        cout << "Ganador eliminación directa" << endl;
        tour.print_winner();
        vector < double > elim = tour.get_winner();

        if (all == elim) {
            results << "quality;" << i << ";" << "same" << ";" << endl;
        } else {
            
            int all_wins = 0;
            int elim_wins = 0;
            tour.reset_scores();
            vector < double > winner = tour.single_match(all, elim);
            if (winner == all) {
                all_wins++;
            } else {
                elim_wins++;
            }
            winner = tour.single_match(elim, all);
            if (winner == all) {
                all_wins++;
            } else {
                elim_wins++;
            }
            if (all_wins > elim_wins) {
                results << "quality;" << i << ";" << "diff;" << "all;" << endl;
            } else if (elim_wins > all_wins) {
                results << "quality;" << i << ";" << "diff;" << "elim;" << endl;
            } else {
                results << "quality;" << i << ";" << "diff;" << "tie;" << endl;

                // Single match no actualiza scores :(
                // int all_goals = 0;
                // int elim_goals = 0;
                // for (int j = 0; j < tour.combinations.size(); j++) {
                //     if (all == tour.combinations[j]) {
                //         all_goals += tour.goals[j];
                //     }
                //     if (elim == tour.combinations[j]) {
                //         elim_goals += tour.goals[j];
                //     }
                // }
                // if (all_goals > elim_goals) {
                //     results << "quality;" << i << ";" << "diff;" << "tie;" << "all;" << endl;
                // } else if (elim_goals > all_goals) {
                //     results << "quality;" << i << ";" << "diff;" << "tie;" << "elim;" << endl;
                // } else {
                //     results << "quality;" << i << ";" << "diff;" << "tie;" << "tie;" << endl;
                // }
            }
        }
    }
}

void test_quality_allvsall_elimination_same_time() {
    string fileName = "test_quality_allvsall_elimination_same_time.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 5; i++) {

        Tournament tour = Tournament(24);
        tour.generate_random_combinations(24);

        tour.play_tournament();
        cout << "Ganador todos contra todos" << endl;
        tour.print_winner();
        vector < double > all = tour.get_winner();
        
        tour.generate_random_combinations(512);
        tour.elimination_cup();
        cout << "Ganador eliminación directa" << endl;
        tour.print_winner();
        vector < double > elim = tour.get_winner();

        if (all == elim) {
            results << "quality_time;" << i << ";" << "same" << ";" << endl;
        } else {
            
            int all_wins = 0;
            int elim_wins = 0;
            tour.reset_scores();
            vector < double > winner = tour.single_match(all, elim);
            if (winner == all) {
                all_wins++;
            } else {
                elim_wins++;
            }
            winner = tour.single_match(elim, all);
            if (winner == all) {
                all_wins++;
            } else {
                elim_wins++;
            }
            if (all_wins > elim_wins) {
                results << "quality_time;" << i << ";" << "diff;" << "all;" << endl;
            } else if (elim_wins > all_wins) {
                results << "quality_time;" << i << ";" << "diff;" << "elim;" << endl;
            } else {
                results << "quality_time;" << i << ";" << "diff;" << "tie;" << endl;

                // Single match no actualiza scores :(
                // int all_goals = 0;
                // int elim_goals = 0;
                // for (int j = 0; j < tour.combinations.size(); j++) {
                //     if (all == tour.combinations[j]) {
                //         all_goals += tour.goals[j];
                //     }
                //     if (elim == tour.combinations[j]) {
                //         elim_goals += tour.goals[j];
                //     }
                // }
                // if (all_goals > elim_goals) {
                //     results << "quality_time;" << i << ";" << "diff;" << "tie;" << "all;" << endl;
                // } else if (elim_goals > all_goals) {
                //     results << "quality_time;" << i << ";" << "diff;" << "tie;" << "elim;" << endl;
                // } else {
                //     results << "quality_time;" << i << ";" << "diff;" << "tie;" << "tie;" << endl;
                // }
            }
        }
    }
}