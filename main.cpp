#include "simulation.hpp"

int main() {
    Simulation::instance().run();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}