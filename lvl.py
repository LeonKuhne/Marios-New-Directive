import pygame
import os.path
import os
from pygame import font
from block import Block


class Level:
    
    FILE_EXT = ".mario"

    def __init__(self, dir_path, grid_size=20):
        self.blocks = []
        self.name = ""
        self.grid_size = grid_size
        self.door_count = 0
        
        self.files = []
        self.dir = dir_path
        self.start()
   
    def compress_vertical(self, column):
        line = ""

        if len(column) == 1:

            # add one
            line += f"{column[0]}"

        elif len(column) > 1:

            # add multiple
            count = 1
            last_val = column[0]
            for val in column[1:]:
                if val == last_val:             # repeat
                    count += 1
                elif count > 1:                 # multiple
                    line += f"{last_val}x{count},"
                    count = 1
                else:
                    line += f"{last_val},"      # single

                # update value
                last_val = val
                
            # add last to line
            line += f"{last_val}x{count}" if count > 1 else f"{last_val}"
        
        return line
    
    def compress_horizontal(self, grid):
        lines = []

        if len(grid) == 1:

            # add one
            line = self.compress_vertical(grid[0])
            lines.append(line)

        elif len(grid) > 1:
            count = 1
            last_line = self.compress_vertical(grid[0])

            # add multiple
            for col in grid[1:]:
                line = self.compress_vertical(col)

                if line == last_line:               # repeat
                    count += 1
                elif count > 1:                     # multiple
                    lines.append(f"({last_line})x{count}")
                    count = 1
                else:                               #single
                    lines.append(last_line)

                # update
                last_line = line

            # add last line
            line = f"({last_line})x{count}" if count > 1 else last_line
            lines.append(line)

        return lines
       

    def decompress_vertical(self, string):
        column = []

        if string:
            for item in string.split(","):
                if "x" in item:
                    # add compressed
                    repeatBlock = item.split("x")
                    block_id = int(repeatBlock[0])
                    count = int(repeatBlock[1])
                    while count > 0:
                        count -= 1
                        column.append(block_id)
    
                else:
                    # add regular
                    column.append(int(item))

        return column

    # ex: (0x3,4)x3
    def decompress_horizontal(self, strings):
        grid = []

        for string in strings:
            
            # decompress horizontally
            count = 1
            if string.startswith("("):
                [string, count] = string[1:].split(")x")
                count = int(count)

            # decompress vertically
            column = self.decompress_vertical(string)
           
            # add
            for i in range(count):
                grid.append(column.copy())

        return grid

    # load a .mario file into a usable 2D array (that uses x, y coordinates)
    def load(self):
        lines = []

        # read in file
        with open(f"{self.dir}/{Level.FILE_EXT}") as f:
            self.name = f.readline().strip()
            print(f"loading '{self.name}' from {self.dir}/{Level.FILE_EXT}...")
            
            for line in f:
                lines.append(line.strip())

        # decompress & save
        values = self.decompress_horizontal(lines)

        # setup blocks
        self.blocks = []
        self.door_count = 0
        for col_id in range(len(values)):
            col_vals = values[col_id]

            cols = []
            for row_id in range(len(col_vals)):
                val = col_vals[row_id]
                text = self.files[val-1] if val > 0 and val < len(self.files) else ""
                block = Block(col_id, row_id, val, text)
                cols.append(block)

                # count doors
                if val > 0: self.door_count += 1

            self.blocks.append(cols)

    def get_files(self):
        files = os.listdir(self.dir)
        files.append("..")
        return files

    def start(self):
        self.files = self.get_files()
        self.navigate()

    def navigate(self, selection=0):
        # select the file
        new_dir = self.dir

        file_name = ""
        if selection in range(1, len(self.files)+1):
            file_name = self.files[selection-1]
            new_dir += f"/{file_name}"

        # open file
        if os.path.isfile(new_dir):
            print(f"Opening file {new_dir}")

        # open directory
        elif os.path.isdir(f"{new_dir}"):
            # update location
            self.name = file_name
            self.dir = new_dir

            if not os.path.exists(f"{self.dir}/{Level.FILE_EXT}"):
                self.blocks = []                # clear the current level
                self.save()                     # create a new save file
                self.files = self.get_files()   # find new files
            
            # load the level
            self.load()


    # save and overwrite file
    def save(self):
        print("saving...")
        values = list(map(lambda x: list(map(lambda y: y.value, x)), self.blocks))

        lines = self.compress_horizontal(values)

        with open(f"{self.dir}/{Level.FILE_EXT}", "w") as f:

            # write level name
            f.write(f"{self.name}\n")

            for line in lines:
                f.write(line + "\n")
    
    def draw(self, screen):
        # draw the level name at the top
        text_font = font.Font(font.get_default_font(), 24)
        text = text_font.render(self.name, True, (255, 255, 255))
        screen.blit(text, (20, 20))

        # draw the options bellow (debug)
        subtext_font = font.Font(font.get_default_font(), 14)
        for i in range(len(self.files)):
            subtext = subtext_font.render(f"{i+1} {self.files[i]}", True, (255, 255, 0))
            screen.blit(subtext, (20, 50+20*i))

        # draw the blocks
        for col_idx in range(0, len(self.blocks)):
            col = self.blocks[col_idx]
            for row_idx in range(0, len(col)):
                block = col[row_idx]
                block.draw(screen, self.grid_size)
  
    def toString(self):
        return f"{self.name}: {self.blocks}"
           

if __name__ == '__main__':
    project_dir = "/home/id/Projects/MND"
    level = Level(project_dir)
    print(level.toString())
