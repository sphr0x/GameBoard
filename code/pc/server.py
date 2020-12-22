import socket, sys, pygame
from pygame.locals import *
from _thread import start_new_thread
from snake import Snake
import threading
import time

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("192.168.2.214", 5678))
    s.listen(9)
    print("waiting for connection!")
except socket.error as e:
    print(e)

def game(g):
    print("Game started")
    t = time.time()
    while not g.gameover:
        if time.time() - t >= 0.05:
            g.snake_move()
            t = time.time()
        g.snake_eats_fruit()
        g.snake_hit_itself()
        g.snake_hit_snake()

def client(conn, g):
    try:
        player = None
        while True:
            data = conn.recv(2).decode()
            if not len(data):
                break
            else:
                player = int(data[1])
                g.control(player, data[0])
            conn.send(g.get_data(player).encode())
        try:
            g.delete_snake(player)
            print("deleted")
            if not len(g.players):
                g.gameover = True        
            conn.close()
        except Exception as e:
            print(e)
    except Exception:
        import traceback
        print(traceback.format_exc())
        g.delete_snake(player)
        print("deleted")
        if not len(g.players):
            g.gameover = True        
        conn.close()

players = 0
g = Snake()

x = threading.Thread(target=game, args=(g,))

x.start()
while not g.gameover:
    conn, addr = s.accept()
    print("connected to", addr)
    player = g.find_missing_player()
    g.add_snake(player)
    conn.send(str(player).encode())
    start_new_thread(client, (conn, g))

x.join()