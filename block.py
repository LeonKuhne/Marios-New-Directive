import pygame
from enum import Enum

class Block:

    class Side(Enum):
        LEFT = 1
        RIGHT = 2
        TOP = 3
        BOTTOM = 4

    BLOCK_TOP_THRESHOLD = 10 # top layer pixel thickness on block
    BLOCK_BOT_THRESHOLD = 10
    BLOCK_SIDE_THRESHOLD = 10
    
    def __init__(self, rect):
        self.rect = rect

    def value_to_color(val):

        if val == 0:
            color = (0, 0, 0) # black
        elif val == 1:
            color = (0, 0, 255) # blue
        elif val == 2:
            color = (0, 255, 0) # green
        elif val == 3:
            color = (255, 255, 0) # yellow
        elif val == 4:
            color = (255, 255, 255) # white
        elif val == -1: 
            color = (255, 150, 0)
        else:
            color = (255, 0, 0) # red

        return color 
    
    def rect(col, row, gs, screen_height):
        x = col * gs
        y = row * gs
        # render with flipped y-axis
        return pygame.Rect(x, screen_height - y - gs, gs, gs)

    def draw(screen, col, row, value, gs, outline=0):
        color = Block.value_to_color(value) if isinstance(value, int) else value
        rect = Block.rect(col, row, gs, screen.get_height())
        pygame.draw.rect(screen, color, rect, outline)


    # CHECK COLLISIONS 
    # the -1 ensures checking bellow the block
    #

    def collides(self, rect):
        return self.rect.colliderect(rect)
    
    def on_top(self, rect):
        diff_top = rect.bottom - self.rect.top
        diff_bot = self.rect.bottom - rect.top

        if diff_top > 0 and diff_top < self.BLOCK_TOP_THRESHOLD:
            return self.Side.TOP
    
        elif diff_bot > 0 and diff_bot < self.BLOCK_BOT_THRESHOLD:
            return self.Side.BOTTOM

        return None

    def on_side(self, rect):
        diff_left = rect.right - self.rect.left
        diff_right = self.rect.right - rect.left

        if diff_left > 0 and diff_left < self.BLOCK_SIDE_THRESHOLD:
            return self.Side.LEFT

        elif diff_right > 0 and diff_right < self.BLOCK_SIDE_THRESHOLD:
            return self.Side.RIGHT

        return None
         
