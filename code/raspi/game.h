#ifndef GAME_H
#define GAME_H

#include <string>

#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  64

//base class for all game
class Game
{
    public:
        int max_players;
        bool has_multi;
        bool is_multi;
        bool is_mmo;
        Game(int max_players, bool has_multi, bool is_mmo=false){
            this->max_players = max_players;
            this->has_multi = has_multi;
            this->is_multi = false;
            this->is_mmo = is_mmo;
        }
        virtual std::string name(){return "Game";};
        virtual int get_winner(){return 0;}
        virtual void draw(Display &display, int player){}
        virtual void control(int player=1, char c='s'){}
        virtual void bot(int player=2){}
        virtual bool main_loop(){return false;}
        virtual std::string get_data(){return "";}
        virtual void set_data(char *msg){}
        virtual void reset(){};
};

#endif