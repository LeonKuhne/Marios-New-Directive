import pygame
from block import Block

class Editor:

    def __init__(self, screen, level, grid_size):
        self.screen = screen
        self.level = level
        self.blocks = self.level.blocks
        self.grid_size = grid_size
        
        # bake paused texture (run 'fc-list' in console to list fonts,
        # use a package manager to install more [arch-linux])
        font = pygame.font.SysFont('Dina', 42)
        self.paused_text = font.render("PAUSED", True, (255, 255, 255)) # 2nd param is antialiasing
    
    def add_block(self, value, col_idx, row_idx):
        # create the new block
        block = Block(col_idx, row_idx, value)

        # extend columns
        while col_idx >= len(self.blocks):
            self.blocks.append([])

        # extend rows
        while row_idx >= len(self.blocks[col_idx]):
            step_idx = len(self.blocks[col_idx])
            self.blocks[col_idx].append(Block(col_idx, step_idx, 0))

        # replace block
        self.blocks[col_idx][row_idx] = block
        
    def draw(self):
        gs = self.grid_size

        # get mouse position
        mouse_x, mouse_y = pygame.mouse.get_pos()

        # highlight block under mouse
        row = int(mouse_x / gs)
        col = int((self.screen.get_height() - mouse_y) / gs) # flip mouse
        
        # DRAW
        # It didn't need to get this complicated :|  - leee

        cursor_surface = pygame.Surface((gs*3, gs*3), pygame.SRCALPHA)

        # draw neighbors
        #for col_offset in range(-1, 1, 2):
        #    Block(col+col_offset, row, -2).draw(cursor_surface, gs, 1)
        #for row_offset in range(-1, 1, 2):
        #    Block(col, row+row_offset, -1).draw(cursor_surface, gs, 1)
        
        # draw cursor block
        Block(col, row, -2).draw(cursor_surface, gs, 5)
        #Block(col, row, 0).draw(cursor_surface, gs)
        #Block(col, row, -1).draw(cursor_surface, gs)

        # calculate positions
        x = row * gs + gs/2
        y = self.screen.get_height() - (col * gs + gs/2) # flip back
        cursor_x = x - cursor_surface.get_width()/2
        cursor_y = y - cursor_surface.get_height()/2
        text_x = self.screen.get_width()/2 - self.paused_text.get_width()/2
        text_y = self.screen.get_height()/2 - self.paused_text.get_height()/2
        
        # render
        self.screen.blit(cursor_surface, (cursor_x, cursor_y))
        self.screen.blit(self.paused_text, (text_x, text_y))

