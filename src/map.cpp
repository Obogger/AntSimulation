

#include "map.hpp"

void Map::generate_map(Vector2<int> size)
{
    map_size = size;

    //Unurderd tile map std::unordered_map<Vector2<int>, Tile>
    tile_map.clear();
    for (int y = 0; y < map_size.y; y++)
    {
        for (int x = 0; x < map_size.x; x++)
        {
            Tile tile;

            //TODO Change this to a better algorithm
            int random_value = rand() % 100;
            if(random_value < 10)
                tile.type = TileType::STONE;
            else if(random_value < 80)
                tile.type = TileType::DIRT;
            else
                tile.type = TileType::EMPTY;

            tile.color = get_color(tile.type);

            tile_map[Vector2<int>(x, y)] = tile;
        }
    }
}

Tile Map::get_tile(Vector2<int> pos)
{
    if(tile_map.find(pos) != tile_map.end())
        return tile_map[pos];

    std::cout << "Tile not found at position: (" << pos.x << ", " << pos.y << ")\n";
    
    Tile tile;
    tile.type = TileType::EMPTY;
    tile.color = get_color(tile.type);
    return tile;
}


int Map::get_tile_size()
{
    return tile_size;
}



SDL_Color Map::get_color(TileType type)
{
    SDL_Color color;
    color = {255, 0, 255, 255}; // Magenta for missing types

    if(type == TileType::EMPTY) color = {0, 0, 0, 255};
    if(type == TileType::DIRT) color = {139, 69, 19, 255}; // Brown
    if(type == TileType::STONE) color = {128, 128, 128, 255}; // Gray



    return color; 

}