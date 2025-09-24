#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>
#include <string>
#include <ctime>

#include "vector2.hpp"
#include "map.hpp"
#include "camera.hpp"

class Simulation 
{
public:
    static Simulation &instance()
    {
        static Simulation instance;
        return instance;
    }

    Simulation(const Simulation &) = delete;
    Simulation &operator=(const Simulation &) = delete;
    Simulation(Simulation &&) = delete;
    Simulation &operator=(Simulation &&) = delete;

    void run(); // Main Simulation loop

private:
    Simulation();
    ~Simulation();

    bool running;                 // Simulation loop control
    Vector2<int> view_port;            //Viewport size
    double delta_time;            // DT in milliseconds
    SDL_Window *window;           // SDL window
    SDL_Renderer *renderer;       // SDL renderer
    TTF_Font *font;
    Vector2<int> map_size;
    std::vector<Colony> colonies;

    Map &map;
    Camera camera; 
    

    // Private methods
    bool initialize();    // Initialize SDL and Simulation systems
    void handle_events(); // Process user input
    void update();        // Update Simulation state
    void render();        // Render everything
};