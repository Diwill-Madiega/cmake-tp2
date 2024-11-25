#include "Game.hpp"

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        return -1;
    }
    return 0;
}
