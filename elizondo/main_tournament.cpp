#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "tournament.hpp"

static const string GENETIC = "genetic";
static const string GRASP = "grasp";

struct Solution
{
    double combination_1;
    double combination_2;
    double combination_3;
    double combination_4;
    double combination_5;
    double combination_6;
    double combination_7;
    double combination_8;
    double combination_9;
    double combination_10;
    int score;
    double duration;
    string algorithm;
    int population;
    int generations;
    int deterministic;
    int elimination;
    int crossover;
    int fitness;
    int distance;
    bool fast;
    int amount;
};

istream &operator>>(istream &is, Solution &solution)
{
    is
        >> solution.combination_1
        >> solution.combination_2
        >> solution.combination_3
        >> solution.combination_4
        >> solution.combination_5
        >> solution.combination_6
        >> solution.combination_7
        >> solution.combination_8
        >> solution.combination_9
        >> solution.combination_10
        >> solution.score
        >> solution.duration
        >> solution.algorithm
        >> solution.population
        >> solution.generations
        >> solution.deterministic
        >> solution.elimination
        >> solution.crossover
        >> solution.fitness
        >> solution.distance
        >> solution.fast
        >> solution.amount;
    return is;
}

ostream &operator<<(ostream &os, Solution &solution)
{
    os
        << solution.combination_1 << ' '
        << solution.combination_2 << ' '
        << solution.combination_3 << ' '
        << solution.combination_4 << ' '
        << solution.combination_5 << ' '
        << solution.combination_6 << ' '
        << solution.combination_7 << ' '
        << solution.combination_8 << ' '
        << solution.combination_9 << ' '
        << solution.combination_10 << ' '
        << solution.score << ' '
        << solution.duration << ' '
        << solution.algorithm << ' '
        << solution.population << ' '
        << solution.generations << ' '
        << solution.deterministic << ' '
        << solution.elimination << ' '
        << solution.crossover << ' '
        << solution.fitness << ' '
        << solution.distance << ' '
        << solution.fast << ' '
        << solution.amount << ' ';
    return os;
}

static vector<string> fileNames;
static vector<double> extractLoads(Solution solution);

void findBestTeamsAndPlay();
void printAndPlayGraspTournament(int population, double distance, bool fast, bool elimination, int amount, int laps);
void printAndPlayGeneticTournament(int population, bool deterministic, bool elimination, bool crossover, bool fitness, int generations, int laps);
void testDiferentesGeneticosVariandoGeneracion(int population);

int main()
{
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);

    testDiferentesGeneticosVariandoGeneracion(64);

    int population = 64;
    bool deterministic = true;
    bool elimination = true;
    bool crossover = true;
    bool fitness = true;
    int generations = 0;
    int laps = 1;

    //GEN - 1 generation and 64 teams 25k''
    printAndPlayGeneticTournament(population, deterministic, elimination, crossover, fitness, generations, laps);

    generations = 1;
    //GEN - 2 generation and 64 teams 49k''
    printAndPlayGeneticTournament(population, deterministic, elimination, crossover, fitness, generations, laps);

    generations = 2;
    //GEN - 3 generation and 64 teams 73k''
    printAndPlayGeneticTournament(population, deterministic, elimination, crossover, fitness, generations, laps);

    double distance = 0.10;
    bool fast = true;
    int amount = 32;
    //GRASP - distance 0.10 and amount 32 536k''
    printAndPlayGraspTournament(population, distance, fast, elimination, amount, laps);

    distance = 0.05;
    //GRASP - distance 0.05 and amount 32 536k''
    printAndPlayGraspTournament(population, distance, fast, elimination, amount, laps);

    findBestTeamsAndPlay();
    return 0;
}

void printAndPlayGraspTournament(int population, double distance, bool fast, bool elimination, int amount, int laps)
{
    Tournament tournament = Tournament(population);
    string file = "experimentos/results/" + GRASP
                  + "_" + to_string(population)
                  + "_" + to_string(distance * 100)
                  + "_" + to_string(fast)
                  + "_" + to_string(elimination)
                  + "_" + to_string(amount)
                  + ".csv";
    fileNames.push_back(file);

    chrono::duration<double, milli> average = (chrono::duration<double, milli>)0;
    ofstream results;
    results.open(file, fstream::out);
    for (int i = 1; i <= laps; i++)
    {
        auto start = chrono::steady_clock::now();
        tournament.grasp(distance, fast, elimination, amount);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        average += diff;
    }

    for (int i = 0; i < tournament.get_combinations().size(); i++)
    {
        for (int j = 0; j < tournament.get_combinations()[i].size(); j++)
        {
            results << tournament.get_combinations()[i][j] << ' ';
        }
        results << tournament.get_scores()[i] << ' ';
        results << chrono::duration<double, milli>(average / laps).count() << ' ';
        results << GRASP << ' ';
        results << 0.0 << ' ';
        results << 0.0 << ' ';
        results << 0.0 << ' ';
        results << elimination << ' ';
        results << 0.0 << ' ';
        results << 0.0 << ' ';
        results << distance << ' ';
        results << fast << ' ';
        results << amount;
        results << endl;
    }
    results.close();
}

void printAndPlayGeneticTournament(int population, bool deterministic, bool elimination, bool crossover, bool fitness, int generations, int laps)
{
    Tournament tournament = Tournament(population);
    string file = "experimentos/results/" + GENETIC
                  + "_" + to_string(population)
                  + "_" + to_string(generations)
                  + "_" + to_string(deterministic)
                  + "_" + to_string(elimination)
                  + "_" + to_string(crossover)
                  + "_" + to_string(fitness)
                  + ".csv";
    fileNames.push_back(file);

    chrono::duration<double, milli> average = (chrono::duration<double, milli>)0;
    ofstream results;
    results.open(file, fstream::out);
    for (int i = 1; i <= laps; i++)
    {
        auto start = chrono::steady_clock::now();
        tournament.genetic(population, deterministic, elimination, crossover, fitness, generations);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        average += diff;
    }

    for (int i = 0; i < tournament.get_combinations().size(); i++)
    {
        for (int j = 0; j < tournament.get_combinations()[i].size(); j++)
        {
            results << tournament.get_combinations()[i][j] << ' ';
        }
        results << tournament.get_scores()[i] << ' ';
        results << chrono::duration<double, milli>(average / laps).count() << ' ';
        results << GENETIC << ' ';
        results << population << ' ';
        results << generations << ' ';
        results << deterministic << ' ';
        results << elimination << ' ';
        results << crossover << ' ';
        results << fitness << ' ';
        results << 0.0 << ' ';
        results << 0.0 << ' ';
        results << 0.0;
        results << endl;
    }
    results.close();
}

void findBestTeamsAndPlay()
{
    vector<vector<double>> combinations;
    string file = "experimentos/results/winners.csv";
    ofstream results;
    results.open(file, fstream::out);

    for (auto fileName : fileNames)
    {
        vector<Solution> solutionTeams;
        ifstream ifs(fileName);
        if (ifs)
        {
            copy(
                istream_iterator<Solution>(ifs),
                istream_iterator<Solution>(),
                back_inserter(solutionTeams));
        }
        else
        {
            cerr << "File not found!" << endl;
        }

        Solution bestTeam = solutionTeams[0];
        for (auto solution : solutionTeams)
        {
            Solution aux = solution;
            if (aux.score > bestTeam.score)
            {
                bestTeam = aux;
            }
        }

        results << bestTeam.combination_1 << ' ';
        results << bestTeam.combination_2 << ' ';
        results << bestTeam.combination_3 << ' ';
        results << bestTeam.combination_4 << ' ';
        results << bestTeam.combination_5 << ' ';
        results << bestTeam.combination_6 << ' ';
        results << bestTeam.combination_7 << ' ';
        results << bestTeam.combination_8 << ' ';
        results << bestTeam.combination_9 << ' ';
        results << bestTeam.combination_10 << ' ';
        results << bestTeam.score << ' ';
        results << bestTeam.duration << ' ';
        results << bestTeam.algorithm << ' ';
        results << bestTeam.population << ' ';
        results << bestTeam.generations << ' ';
        results << bestTeam.deterministic << ' ';
        results << bestTeam.elimination << ' ';
        results << bestTeam.crossover << ' ';
        results << bestTeam.fitness << ' ';
        results << bestTeam.distance << ' ';
        results << bestTeam.fast << ' ';
        results << bestTeam.amount << ' ';
        results << endl;

        combinations.push_back(extractLoads(bestTeam));
    }

    results.close();
    fileNames.push_back(file);

    Tournament tournament = Tournament(combinations.size());
    tournament.reset(combinations.size());
    tournament.get_combinations() = combinations;
    tournament.play_tournament();
    tournament.get_winner();
    tournament.print_winner();
}

vector<double> extractLoads(Solution solution)
{
    return {
        solution.combination_1,
        solution.combination_2,
        solution.combination_3,
        solution.combination_4,
        solution.combination_5,
        solution.combination_6,
        solution.combination_7,
        solution.combination_8,
        solution.combination_9,
        solution.combination_10
    };
}

void testDiferentesGeneticosVariandoGeneracion(int population)
{
    Tournament tournament = Tournament(population);

    vector<double> winner_0 = tournament.genetic_test("deterministic crossover_half scores", population, true, true, true, true, 50);

    Tournament tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("deterministic crossover_half scores");

    vector<double> winner_1 = tournament.genetic_test("deterministic crossover_half goals", population, true, true, true, false, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("deterministic crossover_half goals");

    vector<double> winner_2 = tournament.genetic_test("deterministic crossover_random scores", population, true, true, false, true, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("deterministic crossover_random scores");

    vector<double> winner_3 = tournament.genetic_test("deterministic crossover_random goals", population, true, true, false, false, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("deterministic crossover_random goals");

    vector<double> winner_4 = tournament.genetic_test("no_deterministic crossover_half scores", population, true, false, true, true, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("no_deterministic crossover_half scores");

    vector<double> winner_5 = tournament.genetic_test("no_deterministic crossover_half goals", population, true, false, true, false, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("no_deterministic crossover_half goals");

    vector<double> winner_6 = tournament.genetic_test("no_deterministic crossover_random scores", population, true, false, false, true, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("no_deterministic crossover_random scores");

    vector<double> winner_7 = tournament.genetic_test("no_deterministic crossover_random goals", population, true, false, false, false, 50);

    tournament_generacional = Tournament(tournament.get_generational_winners().size());
    tournament_generacional.get_combinations() = tournament.get_generational_winners();
    tournament_generacional.reset_scores();
    tournament_generacional.play_tournament();
    tournament_generacional.save_score_table("no_deterministic crossover_random goals");

    vector<int> cantVictorias(8, 0);

    for (int i = 0; i < 20; ++i)
    {
        vector<vector<double>> winners{ winner_0, winner_1, winner_2, winner_3, winner_4, winner_5, winner_6, winner_7 };
        tournament.get_combinations() = winners;
        tournament.reset_scores();
        tournament.elimination_cup();
        auto it = max_element(tournament.get_scores().begin(), tournament.get_scores().end());
        auto index = it - tournament.get_scores().begin();
        cantVictorias[index] = cantVictorias[index] + 1;
    }

    string file = "experimentos/results/geneticos_population_" + to_string(population) + "_victorias_ganadores.csv";
    ofstream results;
    results.open(file, fstream::out);

    for (int j = 0; j < cantVictorias.size(); ++j)
    {
        results << cantVictorias[j] << endl;
    }
    results << endl;
    results.close();
}