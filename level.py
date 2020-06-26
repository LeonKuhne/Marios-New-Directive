import pygame

FILE_EXT = ".mario"

class Level:

    blocks = []
    name = "new"

    def __init__(self, path):
        # traverse the path, look for .mario file
        self.load(f"{path}/{FILE_EXT}")
    
    # load a .mario file into a usable 2D array (that uses x, y coordinates)
    def load(self, path):
        grid = []

        with open(path) as f:
            self.name = f.readline().strip()

            for line in f:                      # columns
                column = []

                for item in line.split(","):    # rows

                    # decompress add
                    if "x" in item:
                        repeatBlock = item.split("x")
                        block_id = int(repeatBlock[0])
                        count = int(repeatBlock[1])
                        while count > 0:
                            count -= 1
                            column.append(block_id)

                    # regular add
                    else:
                        column.append(int(item))
            
                # add to grid
                grid.append(column)

        # save
        self.blocks = grid

    def toString(self):
        return f"{self.name}: {self.blocks}"

    def draw(self, screen):
        grid_size = 50

        for col_idx in range(0, len(self.blocks)):
            col = self.blocks[col_idx]
            for row_idx in range(0, len(col)):
                val = col[row_idx]
                x = col_idx * grid_size
                y = row_idx * grid_size

                # set color
                if val == 0:
                    color = (0, 0, 0) # black
                elif val == 1:
                    color = (0, 0, 255) # blue
                elif val == 2:
                    color = (0, 255, 0) # green
                elif val == 3:
                    color = (255, 255, 0) # yellow
                else:
                    color = (255, 255, 255) # default is white

                # render with flipped y-axis
                pygame.draw.rect(screen, color, pygame.Rect(x, screen.get_height()-(y+grid_size), grid_size, grid_size))


if __name__ == '__main__':
    project_dir = "/home/device/Projects/MND"
    level = Level(project_dir)
    print(level.toString())
