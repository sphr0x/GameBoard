import socket, sys, pygame
from pygame.locals import *

SCALE = 5
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0 ,255)
FPS = 30
DISPLAY_WIDTH = 128
DISPLAY_HEIGHT = 64

SIZE = 4 * SCALE

pygame.init()
screen = pygame.display.set_mode((DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE))
clock = pygame.time.Clock()

data = ["2", "2", "3", "8", "10", "12", "16", "10", "10", "2", "100", "40", "30", "15"]

def draw(data):
    global screen
    pygame.draw.rect(screen, GREEN, (int(data[0]) * SCALE, int(data[1]) * SCALE, SIZE, SIZE))
    d1 = data[3: 3 + int(data[2]) * 2]
    p1 = []
    for i in range(len(d1)//2):
        p1.append((int(d1[2 * i]) * SCALE, int(d1[2 * i + 1]) * SCALE, SIZE, SIZE))
    d2 = data[-int(data[3 + int(data[2]) * 2]) * 2:]
    p2 = []
    for i in range(len(d2)//2):
        p2.append((int(d2[2 * i]) * SCALE, int(d2[2 * i + 1]) * SCALE, SIZE, SIZE))
    
    for i in range(len(p1)):
        pygame.draw.rect(screen, BLUE, p1[i])
    for i in range(len(p2)):
        pygame.draw.rect(screen, RED, p2[i])
    return p1, p2

def rect_collided(r1, r2):
    if (r1[0] >= r2[0] and r1[0] <= r2[0] + r2[2]) or (r1[0] + r1[2] >= r2[0] and r1[0] + r1[2] <= r2[0] + r2[2]):
        if (r1[1] >= r2[1] and r1[1] <= r2[1] + r2[3]) or (r1[1] + r1[3] >= r2[1] and r1[1] + r1[3] <= r2[1] + r2[3]):
            return True
    return False

def lose(p1, p2):
    if p1[0][0] < 0 or p1[0][0] > DISPLAY_WIDTH * SCALE:
        return True
    if p1[0][1] < 0 or p1[0][1] > DISPLAY_HEIGHT * SCALE:
        return True
    if p2[0][0] < 0 or p2[0][0] > DISPLAY_WIDTH * SCALE:
        return True
    if p2[0][1] < 0 or p2[0][1] > DISPLAY_HEIGHT * SCALE:
        return True
    for p in p1:
        if rect_collided(p, p2[0]) or rect_collided(p2[0], p):
            return True
    for p in p2:
        if rect_collided(p, p1[0]) or rect_collided(p1[0], p):
            return True    
    return False

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
client_socket.connect(("192.168.2.160", 5678))
player = 2
while True:
    screen.fill((0, 0, 0))
    send_msg = "H" + str(player)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            client_socket.shutdown(socket.SHUT_RDWR)
            client_socket.close()
            pygame.quit()
            sys.exit()
        keys = pygame.key.get_pressed()
        if keys[K_w] or keys[K_UP]:
            send_msg = f'U{player}'
        if keys[K_s] or keys[K_DOWN]:
            send_msg = f'D{player}'
        if keys[K_a] or keys[K_LEFT]:
            send_msg = f'L{player}'
        if keys[K_d] or keys[K_RIGHT]:
            send_msg = f'R{player}'
    client_socket.send(send_msg.encode())
    p1, p2 = draw(client_socket.recv(1024).decode().split(":"))
    if lose(p1, p2):
        send_msg = "O" + str(player)
        client_socket.send(send_msg.encode())
        client_socket.shutdown(socket.SHUT_RDWR)
        client_socket.close()
    pygame.display.flip()
    clock.tick(FPS)