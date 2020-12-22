#include "object.h"

Object::Object(int x, int y, int width, int height, int vel_x, int vel_y){
    this->x = x;
    this->y = y;
    this->vel_x = vel_x;
    this->vel_y = vel_y;
    this->width = width;
    this->height = height;
}

Object::Object(const Object &other){
    this->x = other.x;
    this->y = other.y;
    this->vel_x = other.vel_x;
    this->vel_y = other.vel_y;
    this->width = other.width;
    this->height = other.height;
}

void Object::move(){
    x += vel_x;
    y += vel_y;
}

void Object::move(int v_x, int v_y){
    x += v_x;
    y += v_y;
}

bool Object::is_collided(Object &other){
    if((this->x >= other.x && this->x <= other.x + other.width) || (this->x + this->width >= other.x && this->x + this->width <= other.x + other.width)) {
        if((this->y >= other.y && this->y <= other.y + other.height) || (this->y + this->height >= other.y && this->y + this->height <= other.y + other.height)){
            return true;
        }
    }
    if((other.x >= this->x && other.x <= this->x + this->width) || (other.x + other.width >= this->x && other.x + other.width <= this->x + this->width)) {
        if((other.y >= this->y && other.y <= this->y + this->height) || (other.y + other.height >= this->y && other.y + other.height <= this->y + this->height)){
            return true;
        }
    }
    return false;
}

std::string Object::get_name(){
    return std::to_string(x) + ":" 
            + std::to_string(y) + ":" 
            + std::to_string(vel_x) + ":" 
            + std::to_string(vel_y) +  ":" 
            + std::to_string(width) + ":" 
            + std::to_string(height);
}

void Object::draw(Display &display, bool fill){
    if(fill){
        display.fill_rect(x, y, width, height);
    }
    else {
        display.draw_rect(x, y, width, height);
    }
}

std::string Object::get_position(){
    return std::to_string(x) + ":" + std::to_string(y);
}