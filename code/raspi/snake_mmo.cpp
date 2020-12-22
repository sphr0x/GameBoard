#include "snake_mmo.h"
#include <iostream>
#include <boost/algorithm/string.hpp>

Snake_MMO::Snake_MMO(int max_player, bool has_multi, bool is_mmo) : Game(max_player, has_multi, is_mmo)
{

}

void Snake_MMO::set_data(char *msg){
    data = std::string(msg);
}

void Snake_MMO::draw(Display &display, int player){
    std::vector<std::string> results;
	boost::split(results, data, [](char c){return c == ':';});
    int offset_x = std::stoi(results[0]);
    int offset_y = std::stoi(results[1]);
    for(size_t i=0; i < results.size()/2; ++i){
        int x = std::stoi(results[2*i]);
        int y = std::stoi(results[2*i+1]);
        if(x == offset_x && y == offset_y){
            display.fill_rect(x - offset_x + DISPLAY_WIDTH / 2, y - offset_y + DISPLAY_HEIGHT / 2, SIZE, SIZE);
        }
        else{
            display.draw_rect(x - offset_x + DISPLAY_WIDTH / 2, y - offset_y + DISPLAY_HEIGHT / 2, SIZE, SIZE);
        }
    }
}