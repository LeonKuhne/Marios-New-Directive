import sys, pygame
from lvl import Level
from edit import Editor
from player import Player

FPS = 120

class Browser:
    size = width, height = 1000, 600
    level = None
    
    def __init__(self, grid_size):
        pygame.init()
        pygame.font.init()
        
        self.screen = pygame.display.set_mode(self.size, pygame.RESIZABLE)
        self.clock = pygame.time.Clock()
        self.grid_size = grid_size
        self.player = Player(grid_size)

        self.editor = None

    # for dev purposes
    def draw_grid(self):
        red = pygame.Color("#ff0000")
        width = self.screen.get_width()
        height = self.screen.get_height()

        for x in range(0, width, self.grid_size):    # draw horizontals
            pygame.draw.line(self.screen, red, (x, 0), (x, height))
        for y in range(0, height, self.grid_size):   # draw verticals
            pygame.draw.line(self.screen, red, (0, height-y), (width, height-y))

    def set_level(self, level):
        self.level = level

    def run(self):
        running = True

        while running:
            
            # EVENTS
            #

            for event in pygame.event.get():
                # handle exiting
                if event.type == pygame.QUIT:
                    running = False;

                # handle resize
                if event.type == pygame.VIDEORESIZE:
                    self.screen = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)

                # editor > draw blocks
                if event.type == pygame.MOUSEBUTTONDOWN:
                    mouse_x, mouse_y = pygame.mouse.get_pos()
                    mouse_y = self.screen.get_height()-mouse_y # flip y
                    col = int(mouse_x/self.level.grid_size)
                    row = int(mouse_y/self.level.grid_size)
                    block = event.button-1

                    if self.editor: # button 1: primary
                        self.editor.add_block(block, col, row)
               

                # PRESSED KEYBOARD INPUT
                #
                if event.type == pygame.KEYDOWN:
                    
                    # close game
                    if event.key == pygame.K_w and event.mod == pygame.KMOD_LCTRL or event.key == pygame.K_q:
                        running = False;
                   
                    # toggle editor
                    elif event.key == pygame.K_ESCAPE:
                        if self.level:
                            self.editor = None if self.editor else Editor(self.screen, self.level, self.grid_size)
                        else:
                            print("please select a level before pausing")

                    # player jumps
                    elif event.key == self.player.K_JUMP:
                        if self.player.can_jump() and not self.editor:
                            self.player.jump()

                    # reset position
                    elif event.key == self.player.K_RESET:
                        self.player.reset()

            # HELD KEYBOARD INPUT
            #
            keys = pygame.key.get_pressed()

            # player movement
            if not self.editor:
                if keys[self.player.K_RIGHT]:
                    self.player.right()
                if keys[self.player.K_LEFT]:
                    self.player.left()
                if keys[self.player.K_CROUCH]:
                    self.player.crouch()


            # CHECK COLLISIONS
            #

            if self.level:
                zero_rect = pygame.Rect(0, 0, grid_size, grid_size)

                for col_id in range(len(self.level.blocks)):
                    col = self.level.blocks[col_id]

                    for row_id in range(len(col)):
                        block = self.level.blocks[col_id][row_id]

                        if block > 0:
                            height = self.screen.get_height()
                            gs = self.grid_size
                            rect = zero_rect.move(col_id*gs, height-row_id*gs - gs)
                            self.player.collide_with(rect)

            
            # TICK
            # 
    
            if not self.editor: self.player.tick()


            # DRAW
            # 

            #-------------------------------------------#----# DRAW #----#
            self.screen.fill((0, 0, 0))                 # > background   #
            if self.level: self.level.draw(self.screen) # > level        #
            #self.draw_grid()                            # > grid         #
            if self.editor: self.editor.draw()          # > editor       #
            self.player.draw(self.screen)               # > player       #
            pygame.display.flip()                       # -> render      #
            #-------------------------------------------###------------###

            self.clock.tick(FPS)

        self.level.save()

        
if __name__ == '__main__':
    grid_size = 30
    project_dir = "/home/id/Projects/MND"
    level = Level(project_dir, grid_size)

    game = Browser(grid_size)
    game.set_level(level)
    game.run()
    sys.exit()
