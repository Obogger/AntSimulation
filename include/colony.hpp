#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <SDL2/SDL.h>

#include "vector2.hpp"
#include "nueral_network.hpp"

class Ant
{
public:
    Ant() {}
    ~Ant() {}


    Vector2<double> position;
    int size{10};
    double veocity{1.0};
    double speed{10.0};

    SDL_Color color{0, 0, 0, 255};
    
    private:

};

class Queen : public Ant
{
public:
    Queen() {}
    ~Queen() {}

    SDL_Color color{255, 0, 255, 255};
private:

};

class Colony
{
public:
    Colony(int _layers, int _neurons)
        : neural_network(_layers, _neurons) {}
    ~Colony() {}

    Vector2<int> position;
    int population{0};

    Queen queen;
    std::list<Ant> ants;
    NeuralNetwork neural_network;
private:

};