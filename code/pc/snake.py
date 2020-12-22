import pygame
import random

DISPLAY_WIDTH = 128
DISPLAY_HEIGHT = 64
SNAKE_SIZE = 4
SPEED = 4
MULTIPLE_DISPLAY = 2
MAX_PLAYERS = 10

class Object:
    def __init__(self, player):
        self.x = random.randint(DISPLAY_WIDTH // 4, DISPLAY_WIDTH * MULTIPLE_DISPLAY - DISPLAY_WIDTH // 4)
        self.y = random.randint(DISPLAY_HEIGHT, DISPLAY_HEIGHT * MULTIPLE_DISPLAY - DISPLAY_HEIGHT // 4)
        self.vel_x = random.choice((-SPEED, SPEED))
        self.vel_y = 0
        self.rect = pygame.Rect((self.x, self.y , SNAKE_SIZE, SNAKE_SIZE))

    def move(self):
        self.x += self.vel_x
        self.y += self.vel_y
        if self.x < 0:
            self.x = DISPLAY_WIDTH * MULTIPLE_DISPLAY - SNAKE_SIZE
        if self.x >= DISPLAY_WIDTH * MULTIPLE_DISPLAY:
            self.x = 0
        if self.y < 0:
            self.y = DISPLAY_HEIGHT * MULTIPLE_DISPLAY - SNAKE_SIZE
        if self.y >= DISPLAY_HEIGHT * MULTIPLE_DISPLAY:
            self.y = 0
        self.rect = pygame.Rect((self.x, self.y, SNAKE_SIZE, SNAKE_SIZE))

    def control(self, command):
        if self.vel_y == 0 and command == "U":
            self.vel_y = -SPEED
            self.vel_x = 0
        if self.vel_y == 0 and command == "D":
            self.vel_y = SPEED
            self.vel_x = 0
        if self.vel_x == 0 and command == "L":
            self.vel_x = -SPEED
            self.vel_y = 0
        if self.vel_x == 0 and command == "R":
            self.vel_x = SPEED
            self.vel_y = 0

    def __str__(self):
        return str(self.x) + ":" + str(self.y)


class Snake:
    def __init__(self):
        self.snakes = []
        self.players = []
        self.fruits = [pygame.Rect((random.randint(1, DISPLAY_WIDTH - SNAKE_SIZE + 1) * i, random.randint(1, DISPLAY_HEIGHT - SNAKE_SIZE + 1) * i, SNAKE_SIZE, SNAKE_SIZE)) for i in range(MULTIPLE_DISPLAY + 1)]
        self.gameover = False

    def find_player_index(self, player):
        for i, snake in enumerate(self.snakes):
            if snake[0] == player:
                return i

    def add_snake(self, player):
        self.players.append(player)
        self.snakes.append([player, Object(player)])

    def delete_snake(self, player):
        i = self.find_player_index(player)
        self.players.remove(player)
        self.snakes.pop(i)

    def snake_move(self):
        for snake in self.snakes:
            if len(snake) > 2:
                for i in range(len(snake) - 1, 1, -1):
                    snake[i].x = snake[i - 1].x
                    snake[i].y = snake[i - 1].y
            snake[1].move()
    
    def __str__(self):
        result = ""
        for snake in self.snakes:
            result += str(snake[1]) + ":"
            if len(snake) > 2:
                for i in range(2, len(snake)):
                    result += str(snake[i].x) + ":" + str(snake[i].y) + ":"
        for fruit in self.fruits:
            result += str(fruit.x) + ":" + str(fruit.y) + ":"
        return result[:-1]
    
    def control(self, player, command):
        i = self.find_player_index(player)
        self.snakes[i][1].control(command)

    def find_missing_player(self):
        for i in range(1, MAX_PLAYERS):
            if i not in self.players:
                return i

    def get_data(self, player):
        for snake in self.snakes:
            if snake[0] == player:
                return str(snake[1]) + ':' + str(self)
    
    def snake_eats_fruit(self):
        for snake in self.snakes:
            for fruit in self.fruits:
                if snake[1].rect.colliderect(fruit):
                    snake.append(pygame.Rect((-snake[0] * DISPLAY_WIDTH, -snake[0] * DISPLAY_HEIGHT, SNAKE_SIZE, SNAKE_SIZE)))
                    fruit.x = random.randint(1, DISPLAY_WIDTH - SNAKE_SIZE + 1) * random.randint(1, MULTIPLE_DISPLAY)
                    fruit.y = random.randint(1, DISPLAY_HEIGHT - SNAKE_SIZE + 1) * random.randint(1, MULTIPLE_DISPLAY)

    def snake_hit_itself(self):
        for snake in self.snakes:
            a = len(snake)
            for i in range(4, a):
                if snake[1].rect.colliderect(snake[i]):
                    del snake[2: a - 1]
                    break

    def snake_hit_snake(self):
        for snake1 in self.snakes:
            for snake2 in self.snakes:
                if snake1 is not snake2:
                    if snake1[1].rect.colliderect(snake2[1].rect):
                        if (a := len(snake2)) > 2:
                            del snake2[2 : a - 1]
                        if (b := len(snake1)) > 2:
                            del snake1[2 : b - 1]
                            break
                    for i in range(2, len(snake2)):
                        if snake1[1].rect.colliderect(snake2[i]):
                            if (a := len(snake1)) > 2:
                                del snake1[2 : a - 1]
                                break
