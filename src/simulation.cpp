#include "simulation.hpp"

Simulation::Simulation() : running{false}, view_port{1000, 1000}, delta_time{0.0},
               window{nullptr}, renderer{nullptr}, map_size{Vector2<int>()}, map{Map::instance()}, 
               camera{Camera(Vector2<float>{0,0}, 500)} {}

bool Simulation::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "SDL_TTF could not initialize! SDL_TTF Error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("assets/fonts/montserrat.ttf", 14);

    window = SDL_CreateWindow("TEST GAME",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              view_port.x, view_port.y,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    srand(time(NULL));

    map_size = {100, 100};

    map.generate_map(map_size);
    colonies = map.spawn_colonies(1);

    running = true;
    
    return true;
}

void Simulation::run()
{
    if (!initialize())
        return;

    Uint64 time_now{SDL_GetPerformanceCounter()};
    Uint64 last_time{time_now};

    while (running)
    {
        last_time = time_now;
        time_now = SDL_GetPerformanceCounter();
        delta_time = ((double)(time_now - last_time) / SDL_GetPerformanceFrequency());
        handle_events();
        update();
        render();

        Uint64 frame_time = SDL_GetPerformanceCounter() - time_now;
        Uint64 target_time = SDL_GetPerformanceFrequency() / 144; // 60 FPS
        if (frame_time < target_time)
        {
            SDL_Delay((target_time - frame_time) * 1000 / SDL_GetPerformanceFrequency());
        }
    }

}

void Simulation::handle_events()
{
    static bool key_w = false;
    static bool key_a = false;
    static bool key_s = false;
    static bool key_d = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                key_w = true;
                break;
            case SDLK_a:
                key_a = true;
                break;
            case SDLK_s:
                key_s = true;
                break;
            case SDLK_d:
                key_d = true;
                break;
            case SDLK_LSHIFT:
                camera.speed *= 2;
                break;
            default:
                break;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                key_w = false;
                break;
            case SDLK_a:
                key_a = false;
                break;
            case SDLK_s:
                key_s = false;
                break;
            case SDLK_d:
                key_d = false;
                break;
            case SDLK_LSHIFT:
                camera.speed /= 2;
                break;
            default:
                break;
            }
        }
        if (event.type == SDL_MOUSEWHEEL)
        {
            if (event.wheel.y > 0)
            {
                camera.pos.x -= (view_port.x / 2 - camera.pos.x) / camera.zoom;
                camera.pos.y -= (view_port.y / 2 - camera.pos.y) / camera.zoom;
                camera.zoom += 1;
            }
            else if (event.wheel.y < 0)
            {
                camera.zoom -= 1;
                if (camera.zoom < 1)
                {
                    camera.zoom = 1;
                    continue;
                }
                camera.pos.x += (view_port.x / 2 - camera.pos.x) / (camera.zoom + 1);
                camera.pos.y += (view_port.y / 2 - camera.pos.y) / (camera.zoom + 1);
            }
        }
    }


    Vector2<float> direction;

    if (key_w)
        direction.y -= 1.0f;
    if (key_s)
        direction.y += 1.0f;
    if (key_a == true)
        direction.x -= 1.0f;
    if (key_d)
        direction.x += 1.0f;

    camera.pos.x -= direction.x * camera.speed * delta_time;
    camera.pos.y -= direction.y * camera.speed * delta_time;
}

void Simulation::update()
{
    for(auto &colony : colonies) 
    {   
        colony.neural_network.take_action(delta_time, colony.queen);
        for(auto &ant : colony.ants) 
        {
            colony.neural_network.take_action(delta_time, ant);
        }

    }
}

void Simulation::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render simulation objects herehere
    for(int y = 0; y < map_size.y; y++) 
    {
        for(int x = 0; x < map_size.x; x++) 
        {
            int tile_size = map.get_tile_size();


            int rect_x = x * tile_size * camera.zoom + camera.pos.x;
            int rect_y = y * tile_size * camera.zoom + camera.pos.y;
            
            SDL_Rect tile_rect = {rect_x, rect_y, camera.zoom * tile_size, camera.zoom * tile_size};
            SDL_Color color = map.get_tile(Vector2<int>(x,y)).color;


            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tile_rect);
        }

        for(auto &colony : colonies) 
        {
            int tile_size = map.get_tile_size();

            int colony_rect_x = colony.position.x * tile_size * camera.zoom + camera.pos.x;
            int colony_rect_y = colony.position.y * tile_size * camera.zoom + camera.pos.y;

            SDL_Rect colony_rect = {colony_rect_x, colony_rect_y, camera.zoom * tile_size, camera.zoom * tile_size};
            SDL_Color color = {255, 0, 0, 255};
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &colony_rect);
                
            for(auto &ant : colony.ants) 
            {
                int ant_rect_x = ant.position.x * tile_size * camera.zoom + camera.pos.x;
                int ant_rect_y = ant.position.y * tile_size * camera.zoom + camera.pos.y;

                SDL_Rect ant_rect = {ant_rect_x, ant_rect_y, camera.zoom * ant.size, camera.zoom * ant.size};
                SDL_Color ant_color = ant.color;
                SDL_SetRenderDrawColor(renderer, ant_color.r, ant_color.g, ant_color.b, ant_color.a);
                SDL_RenderFillRect(renderer, &ant_rect);
            }

            int queen_rect_x = colony.queen.position.x * tile_size * camera.zoom + camera.pos.x;
            int queen_rect_y = colony.queen.position.y * tile_size * camera.zoom + camera.pos.y;

            SDL_Rect queen_rect = {queen_rect_x, queen_rect_y, camera.zoom * colony.queen.size, camera.zoom * colony.queen.size};
            SDL_Color queen_color = colony.queen.color;
            SDL_SetRenderDrawColor(renderer, queen_color.r, queen_color.g, queen_color.b, queen_color.a);
            SDL_RenderFillRect(renderer, &queen_rect);  
        }
    }

    SDL_RenderPresent(renderer);
}

Simulation::~Simulation()
{
    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

}