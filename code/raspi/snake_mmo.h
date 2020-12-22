#ifndef SNAKE_MMO_H
#define SNAKE_MMO_H

#include "display.h"
#include "gpio.h"
#include "game.h"
#include "object.h"

#include <string>
#include <vector>
#include <iostream>

#define SIZE 4
#define SPEED 4

class Snake_MMO : public Game
{
    public:
        std::string data;
        Snake_MMO(int max_player = 10, bool has_multi=false, bool is_mmo=true);
        virtual void draw(Display &display, int player=1);
        virtual void set_data(char *msg);
        virtual std::string name(){return "Snake MMO";}
};

#endif
