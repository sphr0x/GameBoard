#include "pong.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>
// Klasse Pong steuert die Bewegung des Balls und der Spieler

Pong::Pong(int max_players, bool has_multi) : Game(max_players, has_multi){
	srand((int)time(0));
	p1 = Object(BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2, BALL_SIZE, BAT_LENGTH);
	p2 = Object(DISPLAY_WIDTH - 2 * BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2, BALL_SIZE, BAT_LENGTH);
	int ball_vel_x = rand()%11 > 5 ? 4 : -4;
	ball = Object((DISPLAY_WIDTH - BALL_SIZE) / 2, (DISPLAY_HEIGHT - BALL_SIZE) / 2, BALL_SIZE, BALL_SIZE, ball_vel_x, 0);
}

// Initialisierung == Startposition des Spiels
void Pong::reset(){
	p1 = Object(BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2, BALL_SIZE, BAT_LENGTH);
	p2 = Object(DISPLAY_WIDTH - 2 * BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2, BALL_SIZE, BAT_LENGTH);
	int ball_vel_x = rand()%11 > 5 ? 4 : -4;
	ball = Object((DISPLAY_WIDTH - BALL_SIZE) / 2, (DISPLAY_HEIGHT - BALL_SIZE) / 2, BALL_SIZE, BALL_SIZE, ball_vel_x, 0);
}

// Wahr, wenn Spieler den Ball trifft
bool Pong::player_hit_ball(int player){
	if(ball.vel_x < 0){
		if(p1.is_collided(ball)){
			return true;
		}
	}
	else {
		if(p2.is_collided(ball)){
			return true;
		}
	}
	return false;
}

// Position von Spieler verändern ( Hitbox )
void Pong::move_player(int player, int distance)
{
	if(player == 1){
		if(distance < 0 && p1.y > 0){
			p1.y += distance;
		}

		if(distance > 0 && p1.y < DISPLAY_HEIGHT - BAT_LENGTH){
			p1.y += distance;
		}
	}
	else{
		if(distance < 0 && p2.y > 0){
			p2.y += distance;
		}

		if(distance > 0 && p2.y < DISPLAY_HEIGHT - BAT_LENGTH)
		{
			p2.y += distance;
		}
	}
}

// Position vom Ball verändern ( Hitbox )
bool Pong::main_loop(){
	bool hit = false;
	if(ball.x > -BALL_SIZE && ball.x < DISPLAY_WIDTH + BALL_SIZE){
		if (ball.y >= DISPLAY_HEIGHT - BALL_SIZE){
			ball.y = DISPLAY_HEIGHT - BALL_SIZE;
			ball.vel_y *= -1;
		}
		if(ball.y <= 0){
			ball.y = 0;
			ball.vel_y *= -1;
		}
		if(player_hit_ball(1) || player_hit_ball(2)){
			hit = true;
			ball.vel_x *= -1;
			ball.vel_y += rand()%9 - 4;
		}
		// Ballposition ändern
		ball.move();
	}
	return hit;
}

// Prüfen, wo der Ball ins Aus ging und Sieger zurückgeben
int Pong::get_winner(){
	//  Ball-X-Position < 0 - Spieler 2 gewinnt
	if (ball.x < 0){
		return 2;
	}
	//  Ball-X-Position > Displaylänge - Spieler 1 gewinnt
	if (ball.x >= DISPLAY_WIDTH){
		return 1;
	}

	return 0;
}

void Pong::draw(Display &display, int player){
	ball.draw(display, true);
	if(player==1){
		p1.draw(display, true);
		p2.draw(display, false);
	}
	else{
		p1.draw(display, false);
		p2.draw(display, true);
	}
}

void Pong::control(int player, char c){
	if(c == 's'){
		if(button_pressed(UP)){
			move_player(player, -PLAYER_SPEED);
		}
		if(button_pressed(DOWN)){
			move_player(player, PLAYER_SPEED);
		}
	}
	else{
		if(c == 'U'){
			move_player(player, -PLAYER_SPEED);
		}
		if(c == 'D'){
			move_player(player, PLAYER_SPEED);
		}
	}
}

void Pong::bot(int player){
	if(player == 2){
		if(ball.vel_x > 0){
			if(p2.y + BAT_LENGTH / 2 > ball.y + BALL_SIZE / 2 + ball.vel_y)
			{
				move_player(2, -PLAYER_SPEED);
			}

			if(p2.y + BAT_LENGTH / 2 < ball.y + BALL_SIZE / 2 + ball.vel_y)
			{
				move_player(2, PLAYER_SPEED);
			}
		}
	}
}

std::string Pong::get_data(){
	std::string result = std::to_string(p1.y) + ":" 
						+ std::to_string(p2.y) + ":" 
						+ std::to_string(ball.x) + ":" 
						+ std::to_string(ball.y) + ":" 
						+ std::to_string(ball.vel_x) + ":" 
						+ std::to_string(ball.vel_y);
	return result;
}						

void Pong::set_data(char *msg){
	std::string data(msg);
	std::vector<std::string> results;
	boost::split(results, data, [](char c){return c == ':';});
	p1.y = std::stoi(results[0]);
	p2.y = std::stoi(results[1]);
	ball.x = std::stoi(results[2]);
	ball.y = std::stoi(results[3]);
	ball.vel_x = std::stoi(results[4]);
	ball.vel_y = std::stoi(results[5]);
}