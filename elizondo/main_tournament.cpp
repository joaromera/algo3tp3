#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <iterator>

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "tournament.cpp"

using namespace std;

struct Solution {
    string algorithm;
    int population;
    int generations;
    int selection;
    int crossover;
    int fitness;
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
};

vector<string> fileNames;

void findBestTeamsAndPlay();
void printAndPlayGeneticTournament(int population, bool selection, bool crossover, bool fitness, int generations, int laps);

istream& operator>>(istream& is, Solution& solution) {
    is 
        >> solution.algorithm 
        >> solution.population 
        >> solution.generations 
        >> solution.selection 
        >> solution.crossover 
        >> solution.fitness 
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
        >> solution.duration;
    return is;
}

ostream& operator<<(ostream& os, Solution& solution) {
    os 
        << solution.algorithm << ' '
        << solution.population << ' '
        << solution.generations << ' '
        << solution.selection << ' '
        << solution.crossover << ' '
        << solution.fitness << ' '
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
        << solution.duration;
    return os;
}

int main(int argc, char **argv) {
    
    //1 generation and 64 teams 24''
    int population = 64;
    bool selection = true;
    bool crossover = true;
    bool fitness = true;
    int generations = 0;
    int laps = 1;
    printAndPlayGeneticTournament(population, selection, crossover, fitness, generations, laps);

    //2 generation and 64 teams 49''
    generations = 1;
    printAndPlayGeneticTournament(population, selection, crossover, fitness, generations, laps);

    //2 generation and 64 teams 46''
    generations = 2;
    printAndPlayGeneticTournament(population, selection, crossover, fitness, generations, laps);
    
    findBestTeamsAndPlay();
    
    return 0;
}

void printAndPlayGeneticTournament(int population, bool selection, bool crossover, bool fitness, int generations, int laps) {
    Tournament tournament = Tournament(population);
    string file = "genetic_" + to_string(population)
        + "_" + to_string(generations)
        + "_" + to_string(selection)
        + "_" + to_string(crossover)
        + "_" + to_string(fitness)
        + ".csv";
    fileNames.push_back(file);

    chrono::duration <double, milli> average = (chrono::duration <double, milli>)0;
    ofstream results;
    results.open(file, fstream::out);
    for (int i = 1; i <= laps; i++) {
        auto start = chrono::steady_clock::now();
        tournament.genetic(population, selection, crossover, fitness, generations);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        average += diff;
    }

    for (int i = 0; i < tournament.combinations.size(); i++) {
        results << "genetic" << ' ';
        results << population << ' ';
        results << generations << ' ';
        results << selection << ' ';
        results << crossover << ' ';
        results << fitness << ' ';
        for (int j = 0; j < tournament.combinations[i].size(); j++) {
            results << tournament.combinations[i][j] << ' ';
        }
        results << tournament.scores[i] << ' ';
        results << chrono::duration <double, milli> (average/laps).count() << endl;
    }
    results.close();    
}

void findBestTeamsAndPlay() {
    vector < vector < double > > teams;
    for(auto fileName : fileNames) {
        vector<Solution> team;
        ifstream ifs(fileName);
        if (ifs) {
            copy(
                istream_iterator<Solution>(ifs),
                istream_iterator<Solution>(),
                back_inserter(team)
            );
        } else {
            cerr << "File not found!" << endl;
        }

        Solution bestTeam = team[0];
        for(auto solution : team) {
            Solution aux = solution;
            if (aux.score > bestTeam.score) {
                bestTeam = aux;
            }
        }
        cout << bestTeam << endl;
        teams.push_back(bestTeam);
    }
}