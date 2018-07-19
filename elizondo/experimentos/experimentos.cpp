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
void test_grasp_local_search();
void test_grasp_local_search_less_ties();
void test_grasp_longrun();
void winners_grasp_longrun();
void winners_grasp_longrun_mod();

int main(int argc, char **argv) {
    
    // time_allvsall(); // Listo
    // time_elimination(); // Listo
    // quality_allvsall_elimination_with_same_teams(); // No hay diferencias notables, pero tal vez mas iteraciones
    // quality_allvsall_elimination_same_time(); // No hay diferencias notables, pero tal vez mas iteraciones
    // test_local_search_iterations_10fast_1slow(); // No hay diferencias notables, Listo
    // test_distance(); // Ganó shrinking 3 de 5. Tiene sentido, pero probar más iteraciones
    // test_distance_fast_neighbourhood(); // 16/50 ganó fixed, 34/50 empataron, achicar el radio no ganó nunca 
    // test_memory_leak(); // Listo
    // test_grasp_local_search(); // Listo 
    // test_grasp_local_search_less_ties(); // Listo
    // test_grasp_longrun(); // Listo
    // winners_grasp_longrun(); // Listo
    // winners_grasp_longrun_mod(); // Listo
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

void test_grasp_local_search() {
    string fileName = "test_grasp_local_search_distance05_8teams.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 10; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 10;
        tournament.iterations_alive_cap = 100;

        auto local_start = chrono::steady_clock::now();
        // vector < double > local = tournament.local_search(start_solution, 0.05, true, true, 8);
        auto local_end = chrono::steady_clock::now();
        auto local_time = local_end - local_start;

        tournament.iterations_cap = 0;
        
        auto grasp_start = chrono::steady_clock::now();
        // vector < double > grasp = tournament.grasp(start_solution, 0.05, true, true, 8);
        auto grasp_end = chrono::steady_clock::now();
        auto grasp_time = grasp_end - grasp_start;

        int local_wins = 0;
        int grasp_wins = 0;
        // vector < double > winner = tournament.single_match(local, grasp);
        // if (winner == local) {
        //     local_wins++;
        // } else {
        //     grasp_wins++;
        // }
        // winner = tournament.single_match(grasp, local);
        // if (winner == local) {
        //     local_wins++;
        // } else {
        //     grasp_wins++;
        // }
        if (local_wins < grasp_wins) {
            results << i << ";grasp;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (local_wins > grasp_wins) {
            results << i << ";local;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        }
    }

    results.close();
}

void test_grasp_longrun() {
    Tournament tournament = Tournament(1);
    tournament.generate_random_combinations(1);
    tournament.iterations_cap = 500;
    tournament.iterations_alive_cap = 10;
    vector < double > grasp = tournament.grasp(0.025, true, true, 32);
}

void winners_grasp_longrun() {
    Tournament tournament = Tournament(30);
    tournament.reset(30);
    tournament.combinations = {
                                {0.875,0.235,0.975,0.215,0.855,0.385,0.455,0.075,0.095,0.285},
                                {0.105,0.025,0.05,0.175,0.115,0.575,0.865,0.815,0.775,0.075},
                                {0.875,1,0.025,0.715,0.675,0.975,0.335,0.075,0.225,0.775},
                                {0.385,1,0.385,0.205,0.475,0.555,0.535,0.655,0.795,0.295},
                                {0.245,0.225,0.815,0.615,0.595,0.595,0.415,0.725,0.705,0.265},
                                {0.595,0.235,0.135,0.735,0.225,0.815,0.555,0.465,0.715,0.195},
                                {0.875,0.495,0.165,0.955,0.495,0.735,0.285,0.315,0.355,0.125},
                                {0.495,0.135,0.345,0.855,0.005,0.355,0.545,0.435,0.125,0.455},
                                {0.375,0.145,0.235,0.445,0.85,0.255,0.915,0.175,0.975,0.425},
                                {0.605,0.725,0.445,0,0.635,0.825,0.385,0.075,0.875,0.865},
                                {0.135,0.1,0.055,0.255,0.685,0.935,0.875,0,0.365,0.235},
                                {0.925,0.525,0.215,0.095,0.355,0.645,0.445,0.705,0.465,0.645},
                                {0.285,0.915,0.175,0.315,0.865,0.035,0.635,0.855,0.095,0.435},
                                {0.055,0.335,0.605,0.255,0.475,0.415,0.755,0.385,0.605,0.455},
                                {0.065,0.425,0.645,0.735,0.755,0.485,0.885,0.285,0.405,0.245},
                                {0.415,0.315,0.035,0.845,0.925,0.965,0.655,0.675,0.605,0.405},
                                {0.685,0.875,0.465,0.065,0.385,0.05,0.875,0.935,1,0.925},
                                {0.4,0.33,0.93,0.47,0.85,0.24,0.57,0.54,0.32,0.88},
                                {0.565,0.445,0.675,0.95,0.295,0.095,0.525,0.655,0.735,0.845},
                                {0.745,0.025,0.915,0.745,0.425,0.265,0.195,0.225,0.725,0.205},
                                {0.655,0.325,0.515,0.835,0.595,0.415,0.295,0.225,0.455,0.205},
                                {0.725,0.205,0.485,0.745,0.205,0.785,0.825,0.785,0.085,0.145},
                                {0.875,0.415,0.515,0.435,0.525,0.115,0.315,0,0.555,0.695},
                                {0.775,0.305,0.675,0.645,0.785,0.425,0.795,0.315,0.715,0.965},
                                {0.945,0.175,0.625,0.225,0.235,0.885,0.845,0.225,0.235,0.725},
                                {0.995,0.125,0.805,0.515,0.915,0.855,1,0.755,0.645,0.865},
                                {0.545,0.365,0.75,0.655,0.935,0.225,0.375,0.275,1,0},
                                {0.205,0.545,0.695,0.05,0.865,0.345,0.415,0.975,0.395,0.505},
                                {0,0.77,0.41,0.31,0.85,0.53,0.91,0.12,0.41,0.27},
                                {0.265,0.565,0.555,0.395,0.855,0.495,0.545,0.735,0.905,1}};
    for (int i = 0; i < 10; i++) {
        vector < vector < bool > > ap (30, vector < bool > (30, false));
        tournament.already_played = ap;
        tournament.play_tournament();
    }
    tournament.print_score_table();
    cout << "AND THE WINNER IS: " << endl;
    tournament.print_winner();
}

void winners_grasp_longrun_mod() {
    Tournament tournament = Tournament(30);
    tournament.reset(13);
    tournament.combinations = {{0.305,0.385,0.215,0.815,0,0.555,0.885,0.385,0.365,0.855},
                                {0.915,0.705,0,0.785,0.715,0.075,0.835,0.365,0.355,0.755},
                                {0.375,0.125,0.415,0.935,0.595,0.575,0.785,0.365,0.055,0.325},
                                {0.295,0.215,0.135,0.05,0.685,0.285,0.165,0,0.545,0.065},
                                {0.905,0.555,1,0.255,0.085,0.075,0.795,0.355,0.685,0.975},
                                {1,0.935,0.05,0.045,0.195,0.445,0.625,0.305,0.915,0.05},
                                {0.915,0.395,0.025,0.025,0.185,0.735,0.675,0.645,0.275,0.135},
                                {0.805,0.805,0.185,0.235,0,0.095,0.575,0.355,0.375,0.595},
                                {0.825,0.725,0.075,0.605,0.555,0.955,0.545,0.245,0.485,0.205},
                                {0.725,0.405,0.745,0.495,0.525,0.425,0.345,0,0.035,0.535},
                                {0.905,0.285,0.225,0.075,0.625,0.185,0.695,0.215,0.265,0.555},
                                {0.875,0.275,0.375,0.535,0.975,0.275,0.745,0.535,1,0.845},
                                {0.925,0.465,0.315,0.835,0.815,0.305,0.545,0.175,0.285,0.645}};
    for (int i = 0; i < 10; i++) {
        vector < vector < bool > > ap (13, vector < bool > (13, false));
        tournament.already_played = ap;
        tournament.play_tournament();
    }
    tournament.print_score_table();
    cout << "AND THE WINNER IS: " << endl;
    tournament.print_winner();
}

void test_grasp_local_search_less_ties() {
    string fileName = "test_grasp_local_search_less_ties_distance025_16teams_20iter.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 50; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 20;
        tournament.iterations_alive_cap = 100;

        auto local_start = chrono::steady_clock::now();
        // vector < double > local = tournament.local_search(start_solution, 0.025, true, true, 16);
        auto local_end = chrono::steady_clock::now();
        auto local_time = local_end - local_start;

        tournament.iterations_cap = 0;
        
        auto grasp_start = chrono::steady_clock::now();
        // vector < double > grasp = tournament.grasp(start_solution, 0.025, true, true, 16);
        auto grasp_end = chrono::steady_clock::now();
        auto grasp_time = grasp_end - grasp_start;

        int local_wins = 0;
        int grasp_wins = 0;
        tournament.reset(2);
        // tournament.combinations.push_back(local);
        // tournament.combinations.push_back(grasp);
        tournament.play_tournament();

        if (tournament.scores[0] < tournament.scores[1]) {
            results << i << ";grasp;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.scores[0] > tournament.scores[1]) {
            results << i << ";local;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.goals[0] < tournament.goals[1]) {
            results << i << ";grasp;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.goals[0] > tournament.goals[1]) {
            results << i << ";local;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        }
    }

    results.close();
}

void test_grasp_criterio_corte() {
    string fileName = "test_grasp_criterio_corte.txt";
    ofstream results;
    results.open(fileName, fstream::out);
    for (int i = 0; i < 50; i ++) {
        Tournament tournament = Tournament(1);
        tournament.generate_random_combinations(1);
        vector < double > start_solution = tournament.combinations[0];
        tournament.iterations_cap = 20;
        tournament.iterations_alive_cap = 5;

        auto local_start = chrono::steady_clock::now();
        vector < double > local = tournament.grasp(0.025, true, true, 64);
        auto local_end = chrono::steady_clock::now();
        auto local_time = local_end - local_start;
        
        auto grasp_start = chrono::steady_clock::now();
        vector < double > grasp = tournament.grasp(0.025, true, true, 64);
        auto grasp_end = chrono::steady_clock::now();
        auto grasp_time = grasp_end - grasp_start;

        int local_wins = 0;
        int grasp_wins = 0;
        tournament.reset(2);
        tournament.combinations.push_back(local);
        tournament.combinations.push_back(grasp);
        tournament.play_tournament();

        if (tournament.scores[0] < tournament.scores[1]) {
            results << i << ";grasp;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.scores[0] > tournament.scores[1]) {
            results << i << ";local;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.goals[0] < tournament.goals[1]) {
            results << i << ";grasp;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else if (tournament.goals[0] > tournament.goals[1]) {
            results << i << ";local;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        } else {
            results << i << ";tie;" << grasp_time.count() << ";" << local_time.count() << ";" <<  endl;
        }
    }

    results.close();
}