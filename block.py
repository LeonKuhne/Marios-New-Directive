import pygame
from pygame import font
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
    
    def __init__(self, col, row, value, text=""):
        self.col = col
        self.row = row
        self.color = Block.value_to_color(value)
        self.value = value
        self.text = text

    def value_to_color(val):
        if val == 0:
            color = (0, 0, 0) # black
        elif val > 0:
            color = (255, 255, 255) # white
        elif val == -2: 
            color = (0, 0, 0, 155) # black
        elif val == -3: 
            color = (255, 155, 0) # orange 
        elif val < 0: 
            color = (0, 255, 0) # green
        return color 
    
    def rect(self, gs, screen_height):
        x = self.col * gs
        y = self.row * gs
        height = gs

        # special case for doors # TODO: this is bad code :)
        if self.value > 0:
            height += gs
            y += gs

        # render with flipped y-axis
        return pygame.Rect(x, screen_height - y - gs, gs, height)

    def draw(self, screen, gs, outline=0):
        height = screen.get_height() 
        rect = self.rect(gs, height)
        pygame.draw.rect(screen, self.color, rect, outline)

        # draw door number
        if self.value > 0:
            text_font = font.Font(font.get_default_font(), 10)
            text = text_font.render(f"{self.value} {self.text}", True, (0, 0, 0))
            text = pygame.transform.rotate(text, 90)
            screen.blit(text, (self.col * gs, height - self.row * gs - text.get_height()))


    # CHECK COLLISIONS 
    # the -1 ensures checking bellow the block
    #

    def collides(rect_a, rect_b):
        return rect_a.colliderect(rect_b)
    
    def on_top(rect_block, rect_top):
        diff_top = rect_top.bottom - rect_block.top
        diff_bot = rect_block.bottom - rect_top.top

        if diff_top > 0 and diff_top < Block.BLOCK_TOP_THRESHOLD:
            return Block.Side.TOP
    
        elif diff_bot > 0 and diff_bot < Block.BLOCK_BOT_THRESHOLD:
            return Block.Side.BOTTOM

        return None

    def on_side(rect_block, rect_side):
        diff_left = rect_side.right - rect_block.left
        diff_right = rect_block.right - rect_side.left

        if diff_left > 0 and diff_left < Block.BLOCK_SIDE_THRESHOLD:
            return Block.Side.LEFT

        elif diff_right > 0 and diff_right < Block.BLOCK_SIDE_THRESHOLD:
            return Block.Side.RIGHT

        return None
         
