#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <queue>
#include <random>
#include <utility>
#include <vector>

#include "auxiliars.hpp"
#include "board_status.hpp"
#include "constants.hpp"
#include "logical_board.cpp"
#include "referee.cpp"

class heavierThan
{
public:
    bool operator()(const std::pair<std::vector<double>, int> &a, const std::pair<std::vector<double>, int> &b) const
    {
        return a.second < b.second;
    }
};

using priorityQueue = std::priority_queue<std::pair<std::vector<double>, int>, std::vector<std::pair<std::vector<double>, int>>, heavierThan>;

class Tournament
{
public:
    explicit Tournament(const int candidates)
    : already_played(std::vector<std::vector<bool>>(candidates, std::vector<bool>(candidates, false)))
    , scores(std::vector<int>(candidates, 0))
    {
        srand(time(NULL));
    }

    // limpia los std::vectors combinations, scores y already_played e inicializa los 2 últimos en 0.
    void reset(const int candidates)
    {
        combinations.clear();
        already_played = std::vector<std::vector<bool>>(candidates, std::vector<bool>(candidates, false));
        scores = std::vector<int>(candidates, 0);
        goals = std::vector<int>(candidates, 0);
    }

    void reset_scores()
    {
        reset(combinations.size());
    }

    // llama a reset y llena combinations con std::vectores de pesos random, para hacer GRASP o para inicializar la población del GENETIC
    void generate_random_combinations(const int candidates)
    {
        reset(candidates);
        for (int i = 0; i < candidates; ++i)
        {
            std::vector<double> combination(weights_amount, 0);
            for (int j = 0; j < combination.size(); ++j)
            {
                combination[j] = rand() % 101 / (double) 100;
            }
            combinations.push_back(combination);
        }
    }

    // devuelve una copia del std::vector que más puntos hizo en scores
    std::vector<double> get_winner() const
    {
        const auto it = max_element(scores.cbegin(), scores.cend());
        const auto index = it - scores.begin();
        return combinations[index];
    }

    // hace competir todas las combinaciones entre sí, actualiza el puntaje en SCORE y marca los equipos que jugaron en ALREADYPLAYED según índices de COMBINATIONS
    void play_tournament()
    {
        for (int i = 0; i < combinations.size(); i++)
        {
            for (int j = 0; j < combinations.size(); j++)
            {
                if (i != j && !already_played[i][j])
                {
                    std::vector<player> teamA;
                    for (int l = 0; l < 3; l++)
                    {
                        player aux = player(l, 0.5);
                        teamA.push_back(aux);
                    }
                    std::vector<player> teamB;
                    for (int l = 0; l < 3; l++)
                    {
                        player aux = player(l, 0.5);
                        teamB.push_back(aux);
                    }

                    //Make teams play
                    Referee referee = Referee(10, 5, 125, teamA, teamB, combinations[i], combinations[j]);
                    const std::string winner = referee.runPlay(IZQUIERDA);

                    goals[i] += referee.getScore(IZQUIERDA);
                    goals[j] += referee.getScore(DERECHA);

                    if (winner == IZQUIERDA)
                    {
                        scores[i] += 3;
                    }
                    else if (winner == DERECHA)
                    {
                        scores[j] += 3;
                    }
                    else
                    {
                        scores[i] += 1;
                        scores[j] += 1;
                    }
                    already_played[i][j] = true;
                }
            }
        }
    }

    // LIGA DE ELIMINACIÓN DIRECTA, se eliminan de a dos, por lo que en cada iteración se divide a la mitad la cantidad de equipos. también actualiza SCORES.
    void play_leg(const std::vector<std::pair<std::vector<double>, int>> &combs)
    {
        // cout << "NOW PLAYING LEG OF " << combs.size() << endl;
        std::vector<std::pair<std::vector<double>, int>> winners;
        for (int i = 0; i < combs.size(); i += 2)
        {
            std::vector<player> teamA;
            std::vector<player> teamB;
            for (int l = 0; l < 3; ++l)
            {
                teamA.emplace_back(l, 0.5);
                teamB.emplace_back(l, 0.5);
            }

            int wins_i = 0;
            int wins_d = 0;

            //Make teams play first match
            Referee referee = Referee(10, 5, 125, teamA, teamB, combs[i].first, combs[i + 1].first);
            std::string winner = referee.runPlay(IZQUIERDA);
            // cout << winner << " TEAM LEFT: " << combs[i].second << " GOALS: " << referee.getScore(IZQUIERDA) << " TEAM RIGHT: " << combs[i+1].second << " GOALS: " << referee.getScore(DERECHA) << endl;
            goals[combs[i].second] += referee.getScore(IZQUIERDA);
            goals[combs[i + 1].second] += referee.getScore(DERECHA);
            int goals_i = referee.getScore(IZQUIERDA);
            int goals_d = referee.getScore(DERECHA) * 2;

            if (winner == IZQUIERDA) ++wins_i;
            if (winner == DERECHA) ++wins_d;

            //Make teams play second match
            Referee referee_2 = Referee(10, 5, 125, teamA, teamB, combs[i].first, combs[i + 1].first);
            winner = referee_2.runPlay(DERECHA);
            // cout << winner << " TEAM LEFT: " << combs[i].second << " GOALS: " << referee_2.getScore(IZQUIERDA) << " TEAM RIGHT: " << combs[i+1].second << " GOALS: " << referee_2.getScore(DERECHA) << endl;
            goals[combs[i].second] += referee_2.getScore(IZQUIERDA);
            goals[combs[i + 1].second] += referee_2.getScore(DERECHA);
            goals_i += referee_2.getScore(IZQUIERDA) * 2;
            goals_d += referee_2.getScore(DERECHA);

            if (winner == IZQUIERDA) ++wins_i;
            if (winner == DERECHA) ++wins_d;

            // Who passes to next round?
            if (wins_i > wins_d)
            {
                scores[combs[i].second] += 3;
                winners.push_back(combs[i]);
            }
            else if (wins_i < wins_d)
            {
                scores[combs[i + 1].second] += 3;
                winners.push_back(combs[i + 1]);
            }
            else
            {
                if (goals_i > goals_d)
                {
                    scores[combs[i].second] += 3;
                    winners.push_back(combs[i]);
                }
                else if (goals_i < goals_d)
                {
                    scores[combs[i + 1].second] += 3;
                    winners.push_back(combs[i + 1]);
                }
                else
                {
                    scores[combs[i].second] += 1;
                    scores[combs[i + 1].second] += 1;
                    winners.push_back(combs[i]);
                }
            }
        }
        if (winners.size() > 1)
        {
            play_leg(winners);
        }
    }

    // Antes de usar PLAY LEG hay que guardar cada combinación con su índice en SCORES
    void elimination_cup()
    {
        std::vector<std::pair<std::vector<double>, int>> indexed_combs;
        indexed_combs.reserve(combinations.size());
        for (int i = 0; i < combinations.size(); ++i)
        {
            indexed_combs.emplace_back(combinations[i], i);
        }
        play_leg(indexed_combs);
    }

    // genera los vecinos del input, juega el torneo, guarda el ganador
    // genera los vecinos del ganador, juega el torneo, si es mejor lo reemplaza
    // si no es mejor que el ganador anterior, termina y devuelve el ganador anterior
    std::vector<double> local_search(std::vector<double> vec, double distance, bool fast, bool elimination, int amount = 64)
    {
        if (fast)
        {
            fast_neighbourhood(vec, distance, amount);
        }
        else
        {
            neighbourhood(vec, distance);
        }

        if (elimination)
        {
            elimination_cup();
        }
        else
        {
            play_tournament();
        }

        std::vector<double> winner = get_winner();
        std::vector<double> old_winner;

        int iterations = 0;
        int iterations_alive = 0;

        while (iterations_alive < 5 && iterations < 10)
        {
            // cout << "NEIGHBOURHOOD: " << iterations << endl;
            ++iterations;
            old_winner = winner;

            if (fast)
            {
                fast_neighbourhood(old_winner, distance, amount);
            }
            else
            {
                neighbourhood(old_winner, distance);
            }

            if (elimination)
            {
                elimination_cup();
            }
            else
            {
                play_tournament();
            }

            winner = get_winner();

            if (winner == old_winner)
            {
                ++iterations_alive;
                // cout << "INVICTUS ! ITERATIONS ALIVE: " << iterations_alive << endl;
            }
            else
            {
                // cout << "NEW SOLUTION!!!" << endl;
                iterations_alive = 0;
            }
        }
        return winner;
    }

    std::vector<double> shrinking_local_search(std::vector<double> vec, double distance, bool fast, bool elimination, bool shrink, int amount = 64)
    {
        if (fast)
        {
            fast_neighbourhood(vec, distance, amount);
        }
        else
        {
            neighbourhood(vec, distance);
        }

        if (elimination)
        {
            elimination_cup();
        }
        else
        {
            play_tournament();
        }

        if (shrink) distance /= 2;

        std::vector<double> winner = get_winner();
        std::vector<double> old_winner;

        int iterations = 0;
        int iterations_alive = 0;

        while (iterations_alive < iterations_alive_cap && iterations < iterations_cap)
        {
            cout << "NEIGHBOURHOOD: " << iterations << endl;

            ++iterations;
            old_winner = winner;

            if (fast)
            {
                fast_neighbourhood(old_winner, distance, amount);
            }
            else
            {
                neighbourhood(old_winner, distance);
            }

            if (elimination)
            {
                elimination_cup();
            }
            else
            {
                play_tournament();
            }

            winner = get_winner();

            if (winner == old_winner)
            {
                ++iterations_alive;
                cout << "INVICTUS ! ITERATIONS ALIVE: " << iterations_alive << endl;
            }
            else
            {
                cout << "NEW SOLUTION!!!" << endl;
                iterations_alive = 0;
            }

            if (shrink) distance /= 2;
        }
        return winner;
    }

    // Unico partido devuelve el ganador, o el izq si empatan
    std::vector<double> single_match(const std::vector<double> &izq, const std::vector<double> &der)
    {
        std::vector<player> teamA;
        std::vector<player> teamB;

        for (int l = 0; l < 3; l++)
        {
            teamA.emplace_back(l, 0.5);
            teamB.emplace_back(l, 0.5);
        }

        Referee referee = Referee(10, 5, 125, teamA, teamB, izq, der);
        const std::string winner = referee.runPlay(IZQUIERDA);

        if (winner == IZQUIERDA)
        {
            return izq;
        }
        else
        {
            return der;
        }
    }

    // Juegan ida y vuelta. Si hay empatan devuelve el que mas goles hizo
    // si hay empate de goles el que mas hizo empezando sin la pelota
    // si aun asi hay empate, devuelve izq
    std::vector<double> home_away_match(const std::vector<double> &izq, const std::vector<double> &der)
    {
        std::vector<player> teamA;
        std::vector<player> teamB;

        for (int l = 0; l < 3; l++)
        {
            teamA.emplace_back(l, 0.5);
            teamB.emplace_back(l, 0.5);
        }

        int izq_w = 0;
        int der_w = 0;
        int izq_g = 0;
        int der_g = 0;

        // First match
        Referee referee = Referee(10, 5, 125, teamA, teamB, izq, der);
        std::string winner = referee.runPlay(IZQUIERDA);
        izq_g += referee.getScore(IZQUIERDA);
        der_g += referee.getScore(DERECHA) * 2;
        if (winner == IZQUIERDA) ++izq_w;
        if (winner == DERECHA) ++der_w;

        // Second match
        Referee referee_2 = Referee(10, 5, 125, teamA, teamB, izq, der);
        winner = referee_2.runPlay(DERECHA);
        izq_g += referee_2.getScore(IZQUIERDA) * 2;
        der_g += referee_2.getScore(DERECHA);
        if (winner == IZQUIERDA) ++izq_w;
        if (winner == DERECHA) ++der_w;

        // Return winner
        if (izq_w > der_w) return izq;
        if (izq_w < der_w) return der;
        if (izq_g > der_g) return izq;
        if (izq_g < der_g) return der;
        return izq;
    }

    std::vector<double> grasp(double distance, bool fast, bool elimination, int amount = 64)
    {
        // std::vector < double > starting
        // reset(1);
        // combinations.push_back(starting);
        generate_random_combinations(1);
        local_search(combinations[0], distance, fast, elimination, amount);

        std::vector<double> winner = get_winner();
        std::vector<double> old_winner;

        int iterations = 0;
        int iterations_alive = 0;

        do
        {
            old_winner = winner;

            generate_random_combinations(1);
            local_search(combinations[0], distance, fast, elimination, amount);

            winner = home_away_match(old_winner, get_winner());
            ++iterations;
            if (winner == old_winner)
            {
                ++iterations_alive;
            }
            else
            {
                // for (int i = 0; i < winner.size(); i++) {
                //     cout << winner[i] << " ";
                // }
                // cout << endl;
                iterations_alive = 0;
            }

        } while (iterations_alive < iterations_alive_cap && iterations < iterations_cap);

        return winner;
    }

    // helper de neighbourhood
    void neighbourhood_recursive(std::vector<double> vec1, int index, double distance)
    {
        if (index < vec1.size())
        {
            std::vector<double> new_vec = vec1;
            new_vec[index] += distance;
            if (new_vec[index] > 1) new_vec[index] = 1;
            if (new_vec[index] < 0) new_vec[index] = 0;
            ++index;

            neighbourhood_recursive(vec1, index, distance);
            neighbourhood_recursive(new_vec, index, distance);
        }
        else if (index == vec1.size())
        {
            combinations.push_back(vec1);
        }
    }

    void update_vector(std::vector<double> &vec, double distance)
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            vec[i] = vec[i] + distance >= 0 ? vec[i] + distance : 0;
        }
    }

    // genera vecindario recursivamente. llama a reset y sobrescribe combinations con los vecinos de vec. todas las combinaciones sumando y restando distance
    void neighbourhood(std::vector<double> vec, double distance)
    {
        int size = pow(2, vec.size());
        reset(size);
        std::vector<double> lower_values = vec;
        update_vector(lower_values, -1.0 * distance);
        neighbourhood_recursive(lower_values, 0, distance * 2);
    }

    // genera 64 vecinos a partir de un std::vector de entrada, suma o resta distance
    void fast_neighbourhood(std::vector<double> comb, double distance, int amount)
    {
        reset(amount);
        combinations.push_back(comb);
        for (int i = 0; i < amount - 1; ++i)
        {
            std::vector<double> neighbour(10, 0);
            for (int j = 0; j < weights_amount; ++j)
            {
                int chance = rand() % 101;
                if (chance < 50)
                {
                    neighbour[j] = comb[j] + distance;
                    if (neighbour[j] > 1) neighbour[j] = 1;
                }
                else
                {
                    neighbour[j] = comb[j] - distance;
                    if (neighbour[j] < 0) neighbour[j] = 0;
                }
            }
            combinations.push_back(neighbour);
        }
    }

    // imprime todos los std::vectores en combinations
    void print_combinations() const
    {
        for (const auto &combination : combinations)
        {
            for (int i = 0; i < combination.size(); ++i)
            {
                cout << combination[i] << " ";
            }
            cout << endl;
        }
    }

    void print_gen_winners() const
    {
        for (const auto &combination : generational_winners)
        {
            for (int i = 0; i < combination.size(); ++i)
            {
                cout << combination[i] << " ";
            }
            cout << endl;
        }
    }

    // imprime la tabla con los puntos de cada combinacion
    void print_score_table() const
    {
        for (int i = 0; i < combinations.size(); ++i)
        {
            cout << "COMBINATION: ";
            for (int j = 0; j < combinations[i].size(); ++j)
            {
                cout << combinations[i][j] << " ";
            }
            cout << "SCORE: " << scores[i] << " GOALS: " << goals[i] << endl;
        }
    }

    void save_score_table(const std::string &name) const
    {
        const std::string &file = "results/" + name + "_ranking.csv";
        ofstream results;
        results.open(file, fstream::out);

        results << "iteration;score;goals;p0;p1;p2;p3;p4;p5;p6;p7;p8;p9" << endl;

        for (int i = 0; i < combinations.size(); i++)
        {
            results << i << ";" << scores[i] << ";" << goals[i] << ";";
            for (int j = 0; j < combinations[i].size(); j++)
            {
                results << combinations[i][j] << ";";
            }
            results << endl;
        }

        results.close();
    }

    // imprime el std::vector con más puntos según score
    void print_winner() const
    {
        const auto it = max_element(scores.cbegin(), scores.cend());
        const auto index = it - scores.cbegin();
        for (int i = 0; i < combinations[0].size(); ++i)
        {
            cout << combinations[index][i] << " ";
        }
        cout << endl;
    }

    // estoy chequeando que todas las combinaciones sean distintas entre sí
    void check_comb() const
    {
        std::cout << combinations.size() << std::endl;
        for (int i = 0; i < combinations.size(); ++i)
        {
            for (int j = 0; j < combinations.size(); ++j)
            {
                if (i != j)
                {
                    if (combinations[i] == combinations[j])
                    {
                        std::cout << "IGUALES" << std::endl;
                    }
                }
            }
        }
    }

    std::vector<double> crossover_half(const std::vector<double> &parent_A, const std::vector<double> &parent_B)
    {
        std::vector<double> child;
        for (int i = 0; i < 3; i++)
        {
            child.push_back(parent_A[i]);
            if (rand() % 101 < 5)
            {
                child[i] = rand() % 101 / (double)100;
            }
        }
        for (int i = 3; i < 6; i++)
        {
            child.push_back(parent_B[i]);
            if (rand() % 101 < 5)
            {// MUTACION
                child[i] = rand() % 101 / (double)100;
            }
        }
        for (int i = 6; i < 10; i++)
        {
            child.push_back(parent_A[i]);
            if (rand() % 101 < 5)
            {
                child[i] = rand() % 101 / (double)100;
            }
        }
        return child;
    }

    std::vector<double> crossover_random(const std::vector<double> &parent_A, const std::vector<double> &parent_B)
    {
        std::vector<double> child;
        for (int i = 0; i < parent_A.size(); i++)
        {
            if (rand() % 101 < 50)
            {
                child.push_back(parent_A[i]);
            }
            else
            {
                child.push_back(parent_B[i]);
            }
            if (rand() % 101 < 5)
            {// MUTACION
                child[i] = rand() % 101 / (double)100;
            }
        }
        return child;
    }

    void crossover(int pPopulation, bool pDeterministic, bool pCrossoverHalf)
    {
        int size = combinations.size();
        if (pDeterministic)
        {
            for (int i = 0; i < size - 1; i += 2)
            {
                if (pCrossoverHalf)
                {
                    combinations.push_back(crossover_half(combinations[i], combinations[i + 1]));
                    combinations.push_back(crossover_half(combinations[i + 1], combinations[i]));
                }
                else
                {
                    combinations.push_back(crossover_random(combinations[i], combinations[i + 1]));
                    combinations.push_back(crossover_random(combinations[i + 1], combinations[i]));
                }
            }
        }
        else
        {
            int parent_A = 0;
            int parent_B = 1;
            if (size == 1) parent_B = 0;
            int parents = size;
            while (size < pPopulation)
            {
                if (pCrossoverHalf)
                {
                    combinations.push_back(crossover_half(combinations[parent_A], combinations[parent_B]));
                    combinations.push_back(crossover_half(combinations[parent_B], combinations[parent_A]));
                }
                else
                {
                    combinations.push_back(crossover_random(combinations[parent_A], combinations[parent_B]));
                    combinations.push_back(crossover_random(combinations[parent_B], combinations[parent_A]));
                }
                parent_A = (parent_A + 1) % parents;
                parent_B = (parent_B + 1) % parents;
                size += 2;
            }
            if (size > pPopulation) combinations.pop_back();
        }
    }

    void selection(bool pDeterministic, bool pScores)
    {
        priorityQueue ranking;
        for (int i = 0; i < combinations.size(); i++)
        {
            if (pScores)
            {
                ranking.push(std::make_pair(combinations[i], scores[i]));// FITNESS POR PUNTOS EN TABLA
            }
            else
            {
                ranking.push(std::make_pair(combinations[i], goals[i]));// FITNESS POR CANTIDAD DE GOLES
            }
        }
        if (pDeterministic)
        {
            int new_size = combinations.size() / 2;
            reset(combinations.size());
            for (int i = 0; i < new_size; i++)
            {
                combinations.push_back(ranking.top().first);
                ranking.pop();
            }
        }
        else
        {
            double max_score = ranking.top().second;
            int new_size = combinations.size() / 2;
            reset(combinations.size());
            while (combinations.size() < new_size && ranking.size() > 0)
            {
                double probability = ranking.top().second / max_score;
                double chance = rand() % 101 / (double)100;
                if (chance < probability)
                {
                    combinations.push_back(ranking.top().first);
                }
                ranking.pop();
            }
        }
    }

    void genetic(int population, bool deterministic, bool elimination, bool crossover_half, bool scores, int generations)
    {
        cout << "IN GENETIC" << endl;
        generational_winners.clear();
        generate_random_combinations(population);
        if (elimination)
        {
            elimination_cup();
        }
        else
        {
            play_tournament();
        }

        std::vector<double> winner = get_winner();
        generational_winners.push_back(winner);
        std::vector<double> old_winner;

        int iterations_alive = 0;
        int iterations = 0;
        while (iterations < generations && iterations_alive < iterations_alive_cap)
        {
            old_winner = winner;
            cout << "ITERATIONS: " << iterations << endl;
            print_score_table();
            selection(deterministic, scores);
            crossover(population, deterministic, crossover_half);
            if (elimination)
            {
                elimination_cup();
            }
            else
            {
                play_tournament();
            }
            winner = get_winner();
            generational_winners.push_back(winner);
            if (winner == old_winner)
            {
                ++iterations_alive;
            }
            else
            {
                iterations_alive = 0;
            }
            ++iterations;
        }
    }

    std::vector<double> genetic_test(const std::string &name, int population, bool elimination, bool deterministic, bool crossover_half, bool scores, int generations)
    {
        const std::string file = "results/" + name + ".csv";
        ofstream results;
        results.open(file, fstream::out);

        cout << "IN GENETIC" << endl;

        generational_winners.clear();
        generate_random_combinations(population);

        if (elimination)
        {
            elimination_cup();
        }
        else
        {
            play_tournament();
        }

        std::vector<double> winner = get_winner();

        for (int j = 0; j < winner.size(); ++j)
        {
            results << winner[j] << ';';
        }
        results << endl;

        std::vector<double> old_winner;

        int iterations_alive = 1;
        int iterations = 0;
        while (iterations < generations && iterations_alive < 5)
        {
            old_winner = winner;
            cout << "ITERATIONS: " << iterations << endl;
            print_score_table();
            selection(deterministic, scores);
            crossover(population, deterministic, crossover_half);
            if (elimination)
            {
                elimination_cup();
            }
            else
            {
                play_tournament();
            }
            winner = get_winner();
            generational_winners.push_back(winner);
            if (winner == old_winner)
            {
                ++iterations_alive;
            }
            else
            {
                iterations_alive = 1;
            }
            ++iterations;

            for (int j = 0; j < winner.size(); ++j)
            {
                results << winner[j] << ';';
            }
            results << endl;
        }

        results.close();

        return winner;
    }

    std::vector<std::vector<double>>& get_combinations()
    {
        return combinations;
    }

    std::vector<std::vector<double>>& get_generational_winners()
    {
        return generational_winners;
    }

    std::vector<int>& get_scores()
    {
        return scores;
    }

private:

    std::vector<std::vector<double>> combinations;
    std::vector<int> scores;
    std::vector<int> goals;
    std::vector<std::vector<bool>> already_played;
    std::vector<std::vector<double>> generational_winners;
    int weights_amount = 10;
    int iterations_cap = 5;
    int iterations_alive_cap = 5;
};