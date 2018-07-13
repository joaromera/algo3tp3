#include <string>
#include <random>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>

#include "tournament.cpp"

void printAndPlayGeneticTournament(int population, bool selection, bool crossover, bool fitness, int generations, int laps);

int main(int argc, char **argv) {
    
    //1 generation and 64 teams 24''
    int population = 64;
    bool selection = true;
    bool crossover = true;
    bool fitness = true;
    int generations = 0;
    int laps = 2;
    printAndPlayGeneticTournament(population, selection, crossover, fitness, generations, laps);

    //2 generation and 64 teams 49''
    generations = 1;
    printAndPlayGeneticTournament(population, selection, crossover, fitness, generations, laps);
    
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
        results << "genetic" << ";";
        results << population << ";";
        results << generations << ";";
        results << selection << ";";
        results << crossover << ";";
        results << fitness << ";";
        for (int j = 0; j < tournament.combinations[i].size(); j++) {
            results << tournament.combinations[i][j] << ";";
        }
        results << tournament.scores[i] << ";";
        results << chrono::duration <double, milli> (average/laps).count() << endl;
    }
    results.close();    
}