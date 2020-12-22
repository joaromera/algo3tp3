#include <string>
#include <random>
#include <vector>
#include <map>

#include "ai_greedy.hpp"
#include "player_controller.hpp"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        player_controller<greedy_player> player(argv[1]);
        player.play();
    }
    return 0;
}