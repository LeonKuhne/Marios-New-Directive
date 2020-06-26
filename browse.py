import sys, pygame
import level

class Browser:
    grid_size = 50
    size = width, height = 1000, 600
    level = None
    
    def __init__(self):
        pygame.init()
        
        self.screen = pygame.display.set_mode(self.size)
        self.clock = pygame.time.Clock()

        self.ball = pygame.image.load("mario.png")
        self.ballrect = self.ball.get_rect()

    # for dev purposes
    def draw_grid(self):
        red = pygame.Color("#ff0000")
        for x in range(0, self.width, self.grid_size):    # draw horizontals
            pygame.draw.line(self.screen, red, (x, 0), (x, self.height))
        for y in range(0, self.height, self.grid_size):   # draw verticals
            pygame.draw.line(self.screen, red, (0, y), (self.width, y))

    def set_level(self, level):
        self.level = level

    def run(self):
        running = True

        while running:
            # handle exiting
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False;
                    break;
    
            speed = [1, 1]
            self.ballrect = self.ballrect.move(speed)
            if self.ballrect.left < 0 or self.ballrect.right > self.width:
                speed[0] = -speed[0]
            if self.ballrect.top < 0 or self.ballrect.bottom > self.height:
                speed[1] = -speed[1]

            # draw
            self.screen.fill((0, 0, 0))
            self.screen.blit(self.ball, self.ballrect)
            self.draw_grid()
            if self.level: self.level.draw(self.screen)
            pygame.display.flip()

            self.clock.tick(120)

        sys.exit()

if __name__ == '__main__':
    project_dir = "/home/device/Projects/MND"
    level = level.Level(project_dir)

    game = Browser()
    game.set_level(level)
    game.run()
