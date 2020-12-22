from pygame.locals import *
import pygame
from pong import *
import socket
import sys
from time import sleep

# Defines
IP = "192.168.002.160"
PORT = 5678

SCALE = 5
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
WHITE = (255, 255, 255)
FPS = 10
WAIT_TIME_OUT = 60

# Initialisierung
pygame.init()
screen = pygame.display.set_mode((DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE))
clock = pygame.time.Clock()
pygame.mixer.music.load('ff7_cc.mp3')
sound = pygame.mixer.Sound('bat_hit_ball.wav')
sound.set_volume(0.1)
pygame.mixer.music.play(-1)
pygame.font.init()
myfont = pygame.font.SysFont('Comic Sans MS', 5 * SCALE)

# IP-Eingabe
ip = []
for i in range(len(IP)):
    if IP[i] != '.':
        ip.append(int(IP[i]))

cheat = False

# Darstellen der Ausgangsposition von Pong auf dem Display.
# BSP: [ |  .   | ]
def draw(game, player):
    global screen
    if player == 1:
        p1_color, p2_color, ball_color = GREEN, RED, BLUE
    else:
        p1_color, p2_color, ball_color = RED, GREEN, BLUE
    pygame.draw.rect(screen, p1_color, (game.p1_position[0] * SCALE, game.p1_position[1] * SCALE, BALL_SIZE * SCALE, BAT_LENGTH * SCALE))
    pygame.draw.rect(screen, p2_color, (game.p2_position[0] * SCALE, game.p2_position[1] * SCALE, BALL_SIZE * SCALE, BAT_LENGTH * SCALE))
    pygame.draw.circle(screen, ball_color, (int((game.ball_position[0] + BALL_SIZE // 2)*SCALE), int((game.ball_position[1] + BALL_SIZE // 2)*SCALE)), int(BALL_SIZE//2 * SCALE))
    #pygame.draw.rect(screen, ball_color, (game.ball_position[0] * SCALE, game.ball_position[1] * SCALE, BALL_SIZE * SCALE, BALL_SIZE * SCALE))

    if cheat:
        positions = calc_ball_positions(game)
        for i in range(len(positions) - 1):
            pygame.draw.line(screen, WHITE, (int(positions[i][0] * SCALE), int(positions[i][1] * SCALE)), (int(positions[i + 1][0] * SCALE), int(positions[i + 1][1] * SCALE)))

# siehe Grafik in Dokumentation für mathematische Berechnungen: 
# Berechnung von X' bei ( Position Y' == 64 oder 0 ) ( Also oben bzw. unten vom Display )
def calc_ball_positions(game):
    x = game.ball_position[0] + BALL_SIZE / 2
    y = game.ball_position[1] + BALL_SIZE / 2
    velx = game.ball_velocity[0]
    vely = game.ball_velocity[1]

    result = [(x, y)]
    for i in range(20):
        if vely > 0:
            x += (DISPLAY_HEIGHT - BALL_SIZE / 2 - y) / vely * velx
            vely *= -1
            y = DISPLAY_HEIGHT - BALL_SIZE / 2
            result.append((x, y))
        elif vely < 0:
            x += (0 + BALL_SIZE / 2 - y) / vely * velx
            vely *= -1
            y = 0 + BALL_SIZE / 2
            result.append((x, y))
        else:
            if velx > 0:
                result.append((128, y))
                break
            else:
                result.append((0, y))
                break
        if x > 128 or x < 0:
            break
    return result

# Siehe Device::menu im Cpp-quellcode
def draw_menu(texts):
    pygame.mixer.music.set_volume(0.1)
    pointer = 1
    # Zeiger bewegen
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            keys = pygame.key.get_pressed()
            if keys[K_w] or keys[K_UP]:
                if pointer > 1:
                    pointer -= 1
            if keys[K_s] or keys[K_DOWN]:
                if pointer < len(texts) - 1:
                    pointer += 1
            if keys[K_RETURN]:
                sleep(0.3)
                pygame.mixer.music.set_volume(0.1)
                return pointer

        screen.fill((0, 0, 0))
        textsurface = myfont.render(texts[0], False, RED)
        screen.blit(textsurface,(40*SCALE, 5*SCALE))

        for i in range(1, len(texts)):
            if pointer == i:
                textsurface = myfont.render(texts[i], False, GREEN)
            else:
                textsurface = myfont.render(texts[i], False, BLUE)
            screen.blit(textsurface, (30*SCALE, (5 + 10*i) * SCALE))
        pygame.display.flip()
        clock.tick(10)

# Siehe Dokumentation
# Lineare Funktion der Flugbahn und x Position des Spielers
# Ermitteln der y Position des Spielers
# Dadurch "Vorhersagen" der Ballposition für den Bot
def bot(game):
    if game.ball_velocity[0] > 0:
        y = game.ball_position[1] + BALL_SIZE // 2
        if game.ball_velocity[1] != 0:
            positions = calc_ball_positions(game)
            a = (positions[-1][1] - positions[-2][1]) / (positions[-1][0] - positions[-2][0])
            b = positions[-1][1] - a * positions[-1][0]
            y = a * game.p2_position[0] + b + BALL_SIZE // 2

        if y <= game.p2_position[1] + BAT_LENGTH / 2:
            game.move_player(2, -PLAYER_SPEED)
        if y >= game.p2_position[1] + BAT_LENGTH / 2:
            game.move_player(2, PLAYER_SPEED)

# Siehe Cpp-dokumentation: Device::single
def single():
    global cheat
    cheat = False
    player = 1
    game = Pong()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            keys = pygame.key.get_pressed()
            if keys[K_w] or keys[K_UP]:
                game.move_player(1, -PLAYER_SPEED)
            if keys[K_s] or keys[K_DOWN]:
                game.move_player(1, PLAYER_SPEED)
            if keys[K_t]:
                cheat = not cheat
        screen.fill((0, 0, 0))
        if game.move_ball():
            sound.play()
        bot(game)
        draw(game, player)
        if game.get_winner():
            sleep(1)
            return player == game.get_winner()
        pygame.display.flip()
        clock.tick(10)

# Siehe Cpp-dokumentation: Device::enter_ip
def enter_ip():
    global ip
    pointer = 0
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            keys = pygame.key.get_pressed()
            if keys[K_a] or keys[K_LEFT]:
                if pointer > 0:
                    pointer -= 1
            if keys[K_d] or keys[K_RIGHT]:
                if pointer < 11:
                    pointer += 1
            if keys[K_w] or keys[K_UP]:
                ip[pointer] += 1
            if keys[K_s] or keys[K_DOWN]:
                ip[pointer] -= 1
            if keys[K_0]:
                ip[pointer] = 0
                if pointer < 11:
                    pointer += 1
            if keys[K_1]:
                ip[pointer] = 1
                if pointer < 11:
                    pointer += 1
            if keys[K_2]:
                ip[pointer] = 2
                if pointer < 11:
                    pointer += 1
            if keys[K_3]:
                ip[pointer] = 3
                if pointer < 11:
                    pointer += 1
            if keys[K_4]:
                ip[pointer] = 4
                if pointer < 11:
                    pointer += 1
            if keys[K_5]:
                ip[pointer] = 5
                if pointer < 11:
                    pointer += 1
            if keys[K_6]:
                ip[pointer] = 6
                if pointer < 11:
                    pointer += 1
            if keys[K_7]:
                ip[pointer] = 7
                if pointer < 11:
                    pointer += 1
            if keys[K_8]:
                ip[pointer] = 8
                if pointer < 11:
                    pointer += 1
            if keys[K_9]:
                ip[pointer] = 9
                if pointer < 11:
                    pointer += 1
            if ip[pointer] == -1:
                ip[pointer] = 9
            if pointer % 3 == 0 and ip[pointer] > 2:
                ip[pointer] = 0
            if ip[pointer] == 10:
                ip[pointer] = 0
            if keys[K_BACKSPACE]:
                ip[pointer] = 0
                if pointer > 0:
                    pointer -= 1
            if keys[K_RETURN]:
                result = ""
                for i in range(0, 4):
                    result += str(ip[3*i]*100 + ip[3*i+1]*10 + ip[3*i+2]) + '.' 
                return result[:-1]
            
        screen.fill((0, 0, 0))
        for i in range(0, len(ip)):
            if pointer == i:
                textsurface = myfont.render(str(ip[i]), False, GREEN)
            else:
                textsurface = myfont.render(str(ip[i]), False, BLUE)
            screen.blit(textsurface,((30 + i*3 + i//3 * SCALE) *SCALE, 20*SCALE))
        pygame.display.flip()
        clock.tick(10)

# Siehe Cpp-dokumentation: Device::join
def join_game():
    global cheat
    cheat = False
    ip = enter_ip()
    screen.fill((0, 0, 0))
    textsurface = myfont.render("connecting", False, BLUE)
    screen.blit(textsurface,(50*SCALE, 20*SCALE))
    pygame.display.flip()
    player = 2
    game = Pong()
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    client_socket.settimeout(WAIT_TIME_OUT)
    client_socket.connect((ip, PORT))
    client_socket.settimeout(None)
    while True:
        screen.fill((0, 0, 0))
        data = "H" + str(player)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                client_socket.shutdown(socket.SHUT_RDWR)
                client_socket.close()
                pygame.quit()
                sys.exit()
            keys = pygame.key.get_pressed()
            if keys[K_w] or keys[K_UP]:
                data = f'U{player}'
            if keys[K_s] or keys[K_DOWN]:
                data = f'D{player}'
            if keys[K_t]:
                cheat = not cheat
        if game.get_winner() != 0:
            client_socket.send(f'O{player}'.encode("utf-8"))
            client_socket.close()
            return player == game.get_winner()

        client_socket.send(data.encode("utf-8"))
        x = client_socket.recv(1024)
        game.set_data(x.decode())
        if game.player_hit_ball(1) or game.player_hit_ball(2):
            sound.play()
        draw(game, player)
        pygame.display.flip()
        clock.tick(FPS)

# Siehe Cpp-dokumentation: Device::server
def host_game():
    global cheat
    cheat = False
    player = 1
    game = Pong()
    host_ip = socket.gethostbyname(socket.gethostname())
    screen.fill((0, 0, 0))
    textsurface = myfont.render(host_ip, False, GREEN)
    screen.blit(textsurface,(50*SCALE, 20*SCALE))
    textsurface = myfont.render("connecting", False, BLUE)
    screen.blit(textsurface,(70*SCALE, 40*SCALE))

    pygame.display.flip()
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((host_ip, PORT))
    server_socket.listen(1)
    server_socket.settimeout(WAIT_TIME_OUT)
    conn, addr = server_socket.accept() 
    server_socket.settimeout(None)
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                server_socket.shutdown(socket.SHUT_RDWR)
                server_socket.close()
            keys = pygame.key.get_pressed()
            if keys[K_w] or keys[K_UP]: 
                game.move_player(1, -PLAYER_SPEED)
            if keys[K_s] or keys[K_DOWN]:
                game.move_player(1, PLAYER_SPEED)
            if keys[K_t]:
                cheat = not cheat
        data = conn.recv(2).decode()
        if len(data):
            screen.fill((0, 0, 0))
            if data[0] == 'U':
                game.move_player(2, -PLAYER_SPEED)
            if data[0] == 'D':
                game.move_player(2, PLAYER_SPEED)
            if data[0] == 'O':
                break
            if game.move_ball():
                sound.play()
            data = game.get_data().encode("utf-8")
            for x in data:
                print(x, end=" ")
            print(game.ball_velocity[1])
            conn.send(data)
            draw(game, player)
            pygame.display.flip()
            clock.tick(FPS)
        else:
            break
    conn.close()
    server_socket.close()
    return True if game.ball_position[0] > 64 else False