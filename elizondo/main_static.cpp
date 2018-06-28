#include <string>
#include <random>
#include <vector>
#include <map>

#include "static_player.hpp"
#include "player_controller.hpp"

int main(int argc, char **argv ) {
    player_controller<static_player> player(argv[1]);
    player.play();
    return 0;
}