import sys, pygame
from lvl import Level
from edit import Editor

class Browser:
    size = width, height = 1000, 600
    level = None
    
    def __init__(self, grid_size):
        pygame.init()
        pygame.font.init()
        
        self.screen = pygame.display.set_mode(self.size, pygame.RESIZABLE)
        self.clock = pygame.time.Clock()
        self.grid_size = grid_size

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
        self.level.grid_size = self.grid_size

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

                if event.type == pygame.KEYDOWN:
                    
                    # close game
                    if event.key == pygame.K_w and event.mod == pygame.KMOD_LCTRL:
                        running = False;
                    
                    # toggle editor
                    if event.key == pygame.K_ESCAPE:
                        if self.level:
                            self.editor = None if self.editor else Editor(self.screen, self.level, self.grid_size)
                        else:
                            print("please select a level before pausing")

                # editor > draw blocks
                if event.type == pygame.MOUSEBUTTONDOWN:
                    mouse_x, mouse_y = pygame.mouse.get_pos()
                    mouse_y = self.screen.get_height()-mouse_y # flip y
                    col = int(mouse_x/self.level.grid_size)
                    row = int(mouse_y/self.level.grid_size)

                    if self.editor: # button 1: primary
                        self.editor.add_block(event.button, col, row)

    
            #-------------------------------------------#----# DRAW #----#
            self.screen.fill((0, 0, 0))                 # > background   #
            if self.level: self.level.draw(self.screen) # > level        #
            self.draw_grid()                            # > grid         #
            if self.editor: self.editor.draw()          # > editor       #
            pygame.display.flip()                       # -> render      #
            #-------------------------------------------###------------###

            self.clock.tick(120)

        self.level.save()

        
if __name__ == '__main__':
    grid_size = 50
    project_dir = "/home/device/Projects/MND"
    level = Level(project_dir)

    game = Browser(grid_size)
    game.set_level(level)
    game.run()
    sys.exit()
