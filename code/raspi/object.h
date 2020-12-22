#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "display.h"
#include <iostream>

class Object
{
    public:
        int x, y, vel_x, vel_y, width, height;
        Object(){}
        Object(int x, int y, int width, int height, int vel_x=0, int vel_y=0);
        Object(const Object &other);
        void move();
        void move(int v_x, int v_y);
        bool is_collided(Object &other);
        std::string get_name();
        void draw(Display &display, bool fill=true);
        std::string get_position();
};

#endif