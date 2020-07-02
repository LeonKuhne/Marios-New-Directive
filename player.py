import pygame
from block import Block

class Player:

    K_JUMP = pygame.K_UP
    K_CROUCH = pygame.K_DOWN
    K_RIGHT = pygame.K_RIGHT
    K_LEFT = pygame.K_LEFT
    K_RESET = pygame.K_r

    WIDTH = 30
    HEIGHT = 50
    MAX_SPEED = 0.05
    COLOR = pygame.Color("#ff33cc")

    F_GRAVITY = 0.0019
    F_FRICTION = 0.002
    F_AIR_FRICTION = 0.01
    F_JUMP = 0.09
    F_MOVE = 0.005

    def __init__(self, grid_size):
        self.vel = {'x': 0, 'y': 0}
        self.side = None
        self.on_ground = False
        self.rect = pygame.Rect(0, 0, self.WIDTH, self.HEIGHT)
        self.grid_size = grid_size

    def reset(self):
        self.rect.x = 0
        self.rect.y = 0

    def jump(self):
        self.vel['y'] = self.F_JUMP
        pass

    def crouch(self):
        pass

    def right(self):
        if self.vel['x'] < self.MAX_SPEED:
            self.vel['x'] += self.F_MOVE

    def left(self):
        if -self.vel['x'] < self.MAX_SPEED:
            self.vel['x'] -= self.F_MOVE

    def collide_with(self, rect):
        block = Block(rect)
        if block.collides(self.rect):

            # get collision side
            top_side = block.on_top(self.rect)
            side = block.on_side(self.rect)
            
            # adjust player pos
            if top_side and not side:
                self.fix_top(top_side, rect)
                self.side = top_side
            if side and not top_side:
                self.fix_sides(side, rect)
                self.side = side
       
            if side and top_side:
                #self.fix_sides(side, rect)
                self.fix_top(top_side, rect)
                self.side = top_side
            
            return True
        return False

    def fix_top(self, side, rect):
        if side == Block.Side.TOP:
            self.rect.bottom = rect.top + 1 # move player slightly inside block
        elif side == Block.Side.BOTTOM:
            self.rect.top = rect.bottom + 1 # move player bellow block

    def fix_sides(self, side, rect):
        if side == Block.Side.LEFT:
            self.rect.right = rect.left
        elif side == Block.Side.RIGHT:
            self.rect.left = rect.right

    def reset_speed(self, side):
        if self.side == Block.Side.RIGHT:
            if self.vel['x'] < 0:
                self.vel['x'] = 0
        elif self.side == Block.Side.LEFT:
            if self.vel['x'] > 0:
                self.vel['x'] = 0
        if self.side == Block.Side.TOP:
            if self.vel['y'] < 0:
                self.vel['y'] = 0
        elif self.side == Block.Side.BOTTOM:
            if self.vel['y'] > 0:
                self.vel['y'] = 0

    def apply_forces(self):
        self.on_ground = self.side == Block.Side.TOP

        if self.on_ground:
            # apply friction
            mod = 1 if self.vel['x'] > 0 else -1
            self.vel['x'] -= self.F_FRICTION * mod

        else:
            # apply gravity
            self.vel['y'] -= self.F_GRAVITY         

    def tick(self):
        # set new speed
        self.reset_speed(self.side)
        self.apply_forces()

        # move player
        x_offset = self.vel['x']*self.grid_size
        y_offset = -self.vel['y']*self.grid_size
        self.rect = self.rect.move(x_offset, y_offset)       

        # reset sides
        self.side = None

    def can_jump(self):
        return self.on_ground

    def draw(self, screen):
        # flip y-axis
        pygame.draw.rect(screen, self.COLOR, self.rect)
                
