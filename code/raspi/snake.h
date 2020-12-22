#ifndef SNAKE_H
#define SNAKE_H

#include "display.h"
#include "gpio.h"
#include "game.h"
#include "object.h"

#include <string>
#include <vector>

#define SIZE 4
#define SPEED 4

class Snake : public Game
{
    public:
        Object fruit;
        std::vector<Object> p1;
        std::vector<Object> p2;
        Snake(int max_players=2, bool has_multi=true);
        void reset();
        void move_snake(std::vector<Object> &p);
        void draw_snake(Display &display, std::vector<Object> &p, bool fill=true);
        void help_control(bool check_input, int &vel1, int &vel2, int speed);
        bool snake_eats_fruit(std::vector<Object> &p, Object &fruit);
        bool lose(std::vector<Object> &p);
        virtual bool main_loop();
        virtual void draw(Display &display, int player=1);
        virtual void control(int player, char c='s');
        virtual std::string name(){return "Snake";}
        virtual int get_winner();

        virtual std::string get_data();
        virtual void set_data(char *msg);
};

#endif