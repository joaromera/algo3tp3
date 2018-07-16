#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>

#include "../tournament.cpp"

void time_allvsall();
void time_elimination();
void quality_allvsall_elimination_with_same_teams();
void quality_allvsall_elimination_same_time();
void test_local_search_iterations_10fast_1slow();
void test_distance();
void test_distance_fast_neighbourhood();
void test_memory_leak();

int main(int argc, char **argv) {
    
    // time_allvsall(); // Listo
    // time_elimination(); // Listo
    // quality_allvsall_elimination_with_same_teams(); // No hay diferencias notables, pero tal vez mas iteraciones
    // quality_allvsall_elimination_same_time(); // No hay diferencias notables, pero tal vez mas iteraciones
    // test_local_search_iterations_10fast_1slow(); // No hay diferencias notables, Listo
    test_distance(); // Ganó shrinking 3 de 5. Tiene sentido, pero probar más iteraciones
    // test_distance_fast_neighbourhood(); // 16/50 ganó fixed, 34/50 empataron, achicar el radio no ganó nunca 
    // test_memory_leak();

    return 0;
}

void time_allvsall() {
    string fileName = "time_allvsall.txt";
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

void time_elimination() {
    string fileName = "time_elimination.txt";
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

void quality_allvsall_elimination_with_same_teams() {
    string fileName = "quality_allvsall_elimination_with_same_teams.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 2; i <= 32; i *= 2) {
        for (int iterations = 0; iterations < 5; iterations++) {
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
}

void quality_allvsall_elimination_same_time() {
    string fileName = "quality_allvsall_elimination_same_time.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    vector < int > all_times  = {2, 3,  6,  9,  12,  24};
    vector < int > elim_times = {4, 8, 32, 64, 128, 512};
    for (int teams = 0; teams < all_times.size(); teams++) {
        for (int i = 0; i < 5; i++) {

            Tournament tour = Tournament(all_times[teams]);
            tour.generate_random_combinations(all_times[teams]);

            tour.play_tournament();
            cout << "Ganador todos contra todos" << endl;
            tour.print_winner();
            vector < double > all = tour.get_winner();
            
            tour.generate_random_combinations(elim_times[teams]);
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
}

void test_distance_fast_neighbourhood() {
    string fileName = "test_distance_fast.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 50; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 4;
        tournament.iterations_alive_cap = 100;

        auto fast_start = chrono::steady_clock::now();
        vector < double > fast = tournament.shrinking_local_search(start_solution, 0.16, true, true, true);
        auto fast_end = chrono::steady_clock::now();
        auto fast_time = fast_end - fast_start;

        tournament.iterations_cap = 4;
        
        auto slow_start = chrono::steady_clock::now();
        vector < double > slow = tournament.shrinking_local_search(start_solution, 0.16, true, true, false);
        auto slow_end = chrono::steady_clock::now();
        auto slow_time = slow_end - slow_start;

        int fast_wins = 0;
        int slow_wins = 0;
        vector < double > winner = tournament.single_match(fast, slow);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        winner = tournament.single_match(slow, fast);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        if (fast_wins < slow_wins) {
            results << i << ";fixed;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else if (fast_wins > slow_wins) {
            results << i << ";shrink;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        }
    }

    results.close();
}

void test_distance() {
    string fileName = "test_distance_slow.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 50; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 4;
        tournament.iterations_alive_cap = 100;

        auto fast_start = chrono::steady_clock::now();
        vector < double > fast = tournament.shrinking_local_search(start_solution, 0.16, false, true, true);
        auto fast_end = chrono::steady_clock::now();
        auto fast_time = fast_end - fast_start;

        tournament.iterations_cap = 4;
        
        auto slow_start = chrono::steady_clock::now();
        vector < double > slow = tournament.shrinking_local_search(start_solution, 0.16, false, true, false);
        auto slow_end = chrono::steady_clock::now();
        auto slow_time = slow_end - slow_start;

        int fast_wins = 0;
        int slow_wins = 0;
        vector < double > winner = tournament.single_match(fast, slow);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        winner = tournament.single_match(slow, fast);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        if (fast_wins < slow_wins) {
            results << i << ";fixed;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else if (fast_wins > slow_wins) {
            results << i << ";shrink;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        }
    }

    results.close();
}

void test_local_search_iterations_10fast_1slow() {
    string fileName = "test_local_search_iterations.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 10; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 10;
        tournament.iterations_alive_cap = 100;

        auto fast_start = chrono::steady_clock::now();
        vector < double > fast = tournament.local_search(start_solution, 0.1, true, true);
        auto fast_end = chrono::steady_clock::now();
        auto fast_time = fast_end - fast_start;

        tournament.iterations_cap = 0;
        
        auto slow_start = chrono::steady_clock::now();
        vector < double > slow = tournament.local_search(start_solution, 0.1, false, true);
        auto slow_end = chrono::steady_clock::now();
        auto slow_time = slow_end - slow_start;

        int fast_wins = 0;
        int slow_wins = 0;
        vector < double > winner = tournament.single_match(fast, slow);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        winner = tournament.single_match(slow, fast);
        if (winner == fast) {
            fast_wins++;
        } else {
            slow_wins++;
        }
        if (fast_wins < slow_wins) {
            results << i << ";EXHAUSTIVE;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else if (fast_wins > slow_wins) {
            results << i << ";FAST;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << slow_time.count() << ";" << fast_time.count() << ";" <<  endl;
        }
    }

    results.close();
}

void test_memory_leak() {
    Tournament tour = Tournament(1);
    tour.generate_random_combinations(2);
    tour.elimination_cup();
}