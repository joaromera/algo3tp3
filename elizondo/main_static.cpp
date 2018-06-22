#include <string>
#include <random>
#include <vector>
#include <map>
#include <iostream>

#include "static_player.hpp"
#include "player_controller.hpp"

int main(int argc, char **argv ) {
    string fileName = "static.log";
    ofstream f;
    f.open(fileName, fstream::out);
    f << "STATIC" << endl;
    f.close();
    player_controller<static_player> player(argv[1]);
    player.play();
    return 0;
}