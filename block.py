import pygame

class Block:

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
        return pygame.Rect(x, screen_height - (y + gs), gs, gs)

    def draw(screen, col, row, value, gs, outline=0):
        color = Block.value_to_color(value) if isinstance(value, int) else value
        rect = Block.rect(col, row, gs, screen.get_height())
        pygame.draw.rect(screen, color, rect, outline)

