#include "device.h"
#include "pong.h"
#include "snake.h"
#include "snake_mmo.h"

int main()
{
    Device device;
    vector<Game*> games;
    vector<string> modes;
    vector<string> game_names;
    vector<string> game_over;
    vector<string> multi;

    int game, mode, chose;
    bool won;

    games.push_back(new Pong());
    games.push_back(new Snake());
    games.push_back(new Snake_MMO());
    game_names.push_back("Gameboard");
    for(auto game : games){
        game_names.push_back(game->name());
    }
    game_names.push_back("Exit");

    games_menu:
        game = device.menu(game_names, true);

        if(!game)goto exit;
        else goto modes_menu;

    modes_menu:
        if(games[game-1]->is_mmo){
            goto join_game;
        }
        modes.clear();
        modes.push_back(games[game-1]->name());
        modes.push_back("Single");
        if(games[game-1]->has_multi)modes.push_back("Multi");
        modes.push_back("Exit");
        mode = device.menu(modes);

        if(!mode)goto games_menu;
        else if(mode == 1)goto single;
        else goto multi_menu;

    single:
        won = device.single(games[game-1]);
        goto game_over_menu;

    multi_menu:
        multi.clear();
        multi.push_back(games[game-1]->name());
        multi.push_back("Host");
        multi.push_back("Join");
        multi.push_back("exit");
        chose = device.menu(multi);
        if(!chose) goto modes_menu;
        else if(chose == 1)goto host_game;
        else goto join_game;

    host_game:
        try{
            won = device.host(games[game-1]);
            goto game_over_menu;
        }
        catch(...){
            goto games_menu;
        }

    join_game:
        try{
            won = device.join(games[game-1]);
            goto game_over_menu;
        }
        catch(...){
            goto games_menu;
        }

    game_over_menu:
        game_over.clear();
        if(won)game_over.push_back("won");
        else game_over.push_back("lost");
        game_over.push_back("play again");
        game_over.push_back("exit");
        chose = device.menu(game_over);
        if(!chose) goto exit;
        else goto games_menu;

    exit:
        device.error("shutting down");
		device.exit();
		//system("sudo shutdown -h now");
    return 0;
}