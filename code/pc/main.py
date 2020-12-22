from pong import *
from display import *
from time import *

# Dokumentation der Hauptschleife ist im Cpp-Quellcode zu finden.
while True:
    choice = None
    won = None
    choice = draw_menu(["GAMEBOARD", "Pong", "Exit"])
    if choice == 2:
        break
    choice = draw_menu(["PONG", "Single", "Multi", "Exit"])
    if choice == 3:
        continue
    elif choice == 1:
        won = single()
    elif choice == 2:
        choice = draw_menu(["PONG", "Join Game", "Host Game", "Exit"])
        if choice == 3:
            continue
        elif choice == 1:
            won = join_game()

            '''try:
            except KeyboardInterrupt:
                break
            except:
                choice = draw_menu(["Error", "Play again", "Exit"])
                if choice == 1:
                    continue
                elif choice == 2:
                    break'''
        elif choice == 2:
            won = host_game()

            '''try:
            except KeyboardInterrupt:
                break
            except:
                choice = draw_menu(["Error", "Play again", "Exit"])
                if choice == 1:
                    continue
                elif choice == 2:
                    break'''

    if won==True:
        choice = draw_menu(["WON", "Play again", "Exit"])
    if won==False:
        choice = draw_menu(["LOST", "Play again", "Exit"])
    if choice == 2:
        break