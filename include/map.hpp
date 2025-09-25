#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <random>

#include <unordered_map>
#include <SDL2/SDL.h>

#include "vector2.hpp"
#include "colony.hpp"

enum class TileType
{
    EMPTY,
    STONE,
    DIRT,
};

struct Tile
{
    TileType type;
    SDL_Color color;
    bool spawnable{true};
    bool wall{true};
};

class Map
{
public:
    static Map &instance()
    {
        static Map instance;
        return instance;
    }
    Map(const Map &) = delete;
    Map &operator=(const Map &) = delete;
    Map(Map &&) = delete;
    Map &operator=(Map &&) = delete;

    void generate_map(Vector2<int> size);
    std::vector<Colony> spawn_colonies(int count);

    int get_tile_size();

    Tile get_tile(Vector2<int> pos);

private:
    Map() {}
    ~Map() {}

    int LAYERS{1};
    int NEURONS{5};

    Vector2<int> map_size;
    int tile_size{16};

    SDL_Color get_color(TileType type);


    std::unordered_map<Vector2<int>, Tile> tile_map;

};