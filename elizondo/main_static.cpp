#include "ai_static.hpp"
#include "player_controller.hpp"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        player_controller<static_player> player(argv[1]);
        player.play();
    }
    return 0;
}