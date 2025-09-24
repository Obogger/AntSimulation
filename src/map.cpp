

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
            else if(random_value < 100)
                tile.type = TileType::DIRT;
            else
                tile.type = TileType::EMPTY;

            tile.color = get_color(tile.type);

            tile_map[Vector2<int>(x, y)] = tile;
        }
    }
}

std::vector<Colony> Map::spawn_colonies(int count)
{
    //TODO Set all spawnable tiles in a list and pick random from that list instead of random coords
    //This will prevent infinite loops when there are no valid spawn points left 
      
    std::vector<Colony> colonies;
    for (int i = 0; i < count; i++)
    {
        int colony_spawn_radius = 3;
        int rand_x = rand() % (map_size.x - colony_spawn_radius - 1);
        int rand_y = rand() % (map_size.y - colony_spawn_radius - 1);

        bool valid_spawn = true;
        for (int y = rand_y; y < colony_spawn_radius + rand_y; y++)
        {
            for (int x = rand_x; x < colony_spawn_radius + rand_x; x++)
            {
                if (!get_tile(Vector2<int>(x, y)).spawnable)
                {
                    i--;
                    valid_spawn = false;
                    break;
                }
            }
            if (!valid_spawn)
                break;
        }

        if (!valid_spawn)
            continue;

        for (int y = rand_y; y < colony_spawn_radius + rand_y; y++)
        {
            for (int x = rand_x; x < colony_spawn_radius + rand_x; x++)
            {
                Tile tile;
                tile.type = TileType::EMPTY;
                tile.color = get_color(tile.type);
                tile.spawnable = false;

                tile_map[Vector2<int>(x, y)] = tile;
            }
        }

        Colony colony(LAYERS, NEURONS);
        colony.position = Vector2<int>(rand_x + colony_spawn_radius / 2, rand_y + colony_spawn_radius / 2);
        colony.queen = Queen();
        colony.queen.position.x = colony.position.x;
        colony.queen.position.y = colony.position.y;

        colonies.push_back(colony);

        std::cout << "Colony spawned at: (" << rand_x << ", " << rand_y << ")\n";
    }

    return colonies;
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