import pygame, socket, sys
from pygame.locals import *

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
client_socket.connect(("192.168.2.214", 5678))

RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0 ,255)
FPS = 30
DISPLAY_WIDTH = 128
DISPLAY_HEIGHT = 64

SIZE = 4

pygame.init()
screen = pygame.display.set_mode((DISPLAY_WIDTH * 16, DISPLAY_HEIGHT * 16))
clock = pygame.time.Clock()

def draw(screen, data):
    data = data.split(":")
    offset_x = int(data[0])
    offset_y = int(data[1])
    for i in range(1, len(data) // 2):
        pygame.draw.rect(screen, BLUE, (int(data[2*i]) * SIZE, int(data[2*i + 1]) * SIZE, SIZE * SIZE, SIZE * SIZE))

player = int(client_socket.recv(1024).decode())
while True:
    screen.fill((0, 0, 0))
    send_msg = f'H{player}'
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
    rcv_msg = client_socket.recv(1024).decode()
    draw(screen, rcv_msg)
    pygame.display.flip()
    clock.tick(FPS)