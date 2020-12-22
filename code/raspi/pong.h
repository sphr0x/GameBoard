#ifndef PONG_H
#define PONG_H

#include "display.h"
#include "gpio.h"
#include <string>
#include "game.h"
#include "object.h"

// Klasse Pong ist für die Spielsteuerung zuständig

#define BALL_SIZE       4
#define BAT_LENGTH      12

#define PLAYER_SPEED	4

class Pong : public Game
{
    public:
        Object p1, p2, ball;

        Pong(int max_players=2, bool has_multi=true);
	    void reset();
	    bool player_hit_ball(int player);
        void move_player(int player, int distance);
        virtual bool main_loop();
        virtual int get_winner();

        virtual void draw(Display &display, int player=1);
        virtual void control(int player, char c='s');

        void bot(int player);
        virtual std::string name(){return "Pong";}
        
        virtual std::string get_data();
        virtual void set_data(char *msg);

};

#endif