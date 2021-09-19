#include "Game.hpp"

int main(int argc, char* args[]) {
    Game game;
    game.init("OpenGS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600);

    return 0;
}