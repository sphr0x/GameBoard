#include "snake.h"
#include <iostream>
#include <boost/algorithm/string.hpp>

Snake::Snake(int max_players, bool has_multi) : Game(max_players, has_multi){
    fruit = Object(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, SIZE, SIZE);
    p1.clear();
    p1.push_back(Object(0, 0, SIZE, SIZE, 4, 0));
    p2.clear();
    p2.push_back(Object(DISPLAY_WIDTH - 2 * SIZE, DISPLAY_HEIGHT - 2 * SIZE, SIZE, SIZE, -4, 0));
}

void Snake::reset(){
    fruit = Object(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, SIZE, SIZE);
    p1.clear();
    p1.push_back(Object(0, 0, SIZE, SIZE, 4, 0));
    p2.clear();
    p2.push_back(Object(DISPLAY_WIDTH - 2 * SIZE, DISPLAY_HEIGHT - 2 * SIZE, SIZE, SIZE, -4, 0));
}

bool Snake::main_loop(){
    move_snake(p1);
    if(!is_multi){
        return snake_eats_fruit(p1, fruit);
    }
    else {
        move_snake(p2);
        return snake_eats_fruit(p1, fruit) || snake_eats_fruit(p2, fruit);
    }
}

void Snake::draw(Display &display, int player){
    fruit.draw(display, false);
    draw_snake(display, p1, true);
    if(is_multi){
        draw_snake(display, p2, false);
    }
}

void Snake::control(int player, char c){
    help_control(button_pressed(UP), p1[0].vel_y, p1[0].vel_x, -SPEED);
    help_control(button_pressed(DOWN), p1[0].vel_y, p1[0].vel_x, SPEED);
    help_control(button_pressed(LEFT), p1[0].vel_x, p1[0].vel_y, -SPEED);
    help_control(button_pressed(RIGHT), p1[0].vel_x, p1[0].vel_y, SPEED);
    if(is_multi){
        help_control(c == 'U', p2[0].vel_y, p2[0].vel_x, -SPEED);
        help_control(c == 'D', p2[0].vel_y, p2[0].vel_x, SPEED);
        help_control(c == 'L', p2[0].vel_x, p2[0].vel_y, -SPEED);
        help_control(c == 'R', p2[0].vel_x, p2[0].vel_y, SPEED);
    }
}

int Snake::get_winner(){
    if(lose(p1))return 2;
    if(is_multi){
        if(lose(p2)){
            return 1;
        }
        if (p2[0].is_collided(p1[0])){
            return -1;
        }
        for(size_t i = 1; i < p1.size(); ++i){
            if(p2[0].is_collided(p1[i])){
                return 1;
            }
        }
        for(size_t i = 1; i < p2.size(); ++i){
            if(p1[0].is_collided(p2[i])){
                return 2;
            }
        }
    }
    return 0;
}

void Snake::move_snake(std::vector<Object> &p){
    if(p.size()>1){
        for(size_t i=p.size()-1; i>0; --i){
            p[i].x = p[i - 1].x;
            p[i].y = p[i-1].y;
        }
    }
    p[0].move();
}

void Snake::draw_snake(Display &display, std::vector<Object> &p, bool fill){
    for(auto &e: p){
        e.draw(display, fill);
    }
}

bool Snake::snake_eats_fruit(std::vector<Object> &p, Object &fruit){
    if(p[0].is_collided(fruit)){
        p.push_back(Object(-4, -4, 4, 4));
        do{
            fruit.x = rand()%(DISPLAY_WIDTH-SIZE-1);
        }while(fruit.x % SPEED != 0);
        do{
            fruit.y = rand()%(DISPLAY_HEIGHT-SIZE-1);
        }while(fruit.y % SPEED != 0);
        return true;
    }
    return false;
}

void Snake::help_control(bool check_input, int &vel1, int &vel2, int speed){
    if(check_input && vel1 == 0){
        vel2 = 0;
        vel1 = speed;
    }
}

bool Snake::lose(std::vector<Object> &p){
    if(p[0].x < 0 || p[0].x > DISPLAY_WIDTH){
        return true;
    }
    if(p[0].y < 0 || p[0].y >= DISPLAY_HEIGHT){
        return true;
    }
    if(p.size() > 3){
        for(size_t i=3; i< p.size(); ++i){
            if(p[i].is_collided(p[0])){
                return true;
            }
        }
    }
    return false;
}

std::string Snake::get_data(){
    std::string result = fruit.get_position();
    result += ":" + std::to_string(p1.size());
    for(auto &e : p1){
        result += ":" + e.get_position();
    }
    result += ":" + std::to_string(p2.size());
    for(auto &e : p2){
        result += ":" + e.get_position();
    }
    return result;
}

void Snake::set_data(char *msg){
    std::string data(msg);
	std::vector<std::string> results;
	boost::split(results, data, [](char c){return c == ':';});
    fruit.x = std::stoi(results[0]);
    fruit.y = std::stoi(results[1]);
    int p1_length = std::stoi(results[2]);
    for(size_t i=0; i< p1_length; ++i){
        if(p1.size()<i){
            p1.push_back(Object(std::stoi(results[2*i + 3]), std::stoi(results[2*i + 4]), SIZE, SIZE));
        }
        else{
            p1[i].x = std::stoi(results[2*i + 3]);
            p1[i].y = std::stoi(results[2*i + 4]);
        }
    }
    int p2_length = std::stoi(results[3+2*p1_length]);
    for(size_t i=0; i< p2_length; ++i){
        if(p2.size()<i){
            p2.push_back(Object(std::stoi(results[2*i + 4 + 2 * p1_length]), std::stoi(results[2*i + 5 + 2 * p1_length]), SIZE, SIZE));
        }
        else{
            p2[i].x = std::stoi(results[2*i + 4 + 2 * p1_length]);
            p2[i].y = std::stoi(results[2*i + 5 + 2 * p1_length]);
        }
    }
}