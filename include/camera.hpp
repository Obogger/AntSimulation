#pragma once

#include "vector2.hpp"



class Camera 
{
    public:
        Camera(Vector2<float> _pos, int _speed) : pos{_pos}, speed{_speed} {};
        ~Camera() = default;

        Vector2<float> pos;
        int speed;
        int zoom = 1;


};