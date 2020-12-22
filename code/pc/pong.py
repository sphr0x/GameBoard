import random

# Defines
DISPLAY_WIDTH = 128
DISPLAY_HEIGHT = 64
BALL_SIZE = 4
BAT_LENGTH = 12
PLAYER_SPEED = 4

# Funktion der Klasse Pong sind im Cpp-Quellcode dokumentiert.
class Pong:
    def __init__(self):
        self.ball_position = [(DISPLAY_WIDTH - BALL_SIZE)/2, (DISPLAY_HEIGHT - BALL_SIZE) / 2]
        self.p1_position = [BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2]
        self.p2_position = [DISPLAY_WIDTH - 2 * BALL_SIZE, (DISPLAY_HEIGHT - BAT_LENGTH) / 2]
        self.ball_velocity = [random.choice((-4, 4)), 0]

    def reset(self):
        self.ball_position = [(DISPLAY_WIDTH - BALL_SIZE)/2, (DISPLAY_HEIGHT - BALL_SIZE) / 2]
        self.p1_position = [0, (DISPLAY_HEIGHT - BAT_LENGTH) / 2]
        self.p2_position = [DISPLAY_WIDTH - BALL_SIZE - 1, (DISPLAY_HEIGHT - BAT_LENGTH) / 2]
        self.ball_velocity = [random.choice((-4, 4)), 0]

    def player_hit_ball(self, player):
        if player == 1 and self.ball_velocity[0] < 0:
            if self.ball_position[0] <= self.p1_position[0] + BALL_SIZE:
                if self.ball_position[1] <= self.p1_position[1] + BAT_LENGTH and self.ball_position[1] >= self.p1_position[1]:
                    return True
                if self.ball_position[1] + BALL_SIZE  <= self.p1_position[1] + BAT_LENGTH and self.ball_position[1] + BALL_SIZE >= self.p1_position[1]:
                    return True
        elif player == 2 and self.ball_velocity[0] > 0:
            if self.ball_position[0] + BALL_SIZE >= self.p2_position[0]:
                if self.ball_position[1] <= self.p2_position[1] + BAT_LENGTH and self.ball_position[1] >= self.p2_position[1]:
                    return True
                if self.ball_position[1] + BALL_SIZE <= self.p2_position[1] + BAT_LENGTH and self.ball_position[1] + BALL_SIZE >= self.p2_position[1]:
                    return True
        return False

    def move_player(self, player, distance):
        if player == 1:
            if distance < 0 and self.p1_position[1] > 0:
                self.p1_position[1] += distance
            if distance > 0 and self.p1_position[1] < DISPLAY_HEIGHT - BAT_LENGTH:
                self.p1_position[1] += distance
        else:
            if distance < 0 and self.p2_position[1] > 0:
                self.p2_position[1] += distance
            if distance > 0 and self.p2_position[1] < DISPLAY_HEIGHT - BAT_LENGTH:
                self.p2_position[1] += distance

    def move_ball(self):
        hit = False
        if self.ball_position[0] > -BALL_SIZE and self.ball_position[0] < DISPLAY_WIDTH + BALL_SIZE:
            if self.ball_position[1] >= DISPLAY_HEIGHT - BALL_SIZE:
                self.ball_position[1] = DISPLAY_HEIGHT - BALL_SIZE
                self.ball_velocity[1] *= -1
            if self.ball_position[1] <= 0:
                self.ball_position[1] = 0
                self.ball_velocity[1] *= -1
            if self.player_hit_ball(1) or self.player_hit_ball(2):
                hit = True
                self.ball_velocity[0] *= -1
                self.ball_velocity[1] += random.randint(-4, 4)
                if self.ball_velocity[1] > 2 * PLAYER_SPEED:
                    self.ball_velocity[1] = 2 * PLAYER_SPEED
                elif self.ball_velocity[1] < - 2 * PLAYER_SPEED:
                    self.ball_velocity[1] = - 2 * PLAYER_SPEED
            self.ball_position[0] += self.ball_velocity[0]
            self.ball_position[1] += self.ball_velocity[1]
        return hit

    def get_winner(self):
        if self.ball_position[0] <= 0 or self.ball_position[0] > 200:
            return 2
        if self.ball_position[0] >= DISPLAY_WIDTH:
            return 1
        return 0

    def get_data(self):
        result = str(self.p1_position[1]) + ":" + str(self.p2_position[1]) + ":" + str(self.ball_position[0]) + ":" + str(self.ball_position[1])+ ":" + str(self.ball_velocity[0]) + ":" + str(self.ball_velocity[1])
        return result

    def set_data(self, data):
        data = data.split(":")
        self.p1_position[1] = int(data[0])
        self.p2_position[1] = int(data[1])
        self.ball_position[0] = int(data[2])
        self.ball_position[1] = int(data[3])
        self.ball_velocity[0] = int(data[4])
        self.ball_velocity[1] = int(data[5])