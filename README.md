# Conway's Game of Life

Conway Game of Life is a classic simulation game where the cells follow by four simple rules

- Any live cell with fewer than two live neighbors dies, as if by underpopulation.
- Any live cell with two or three live neighbors lives on to the next generation.
- Any live cell with more than three live neighbors dies, as if by overpopulation.
- Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Albeit being simple when left alone in the right condition, the placement of cells produce insanely mesmerizing patterns.

In this particular project, I will implement hard boundaries in the sense that the simulation will wrap around from one border from the right to the border to the left.

The plan is also to make the project works on multiple platforms (Desktop OSes and Web)

You can try it out on [itch.io](https://dreamerghostone.itch.io/conway-game-of-life)

## Controls

- LMB - Place/Delete Cell
- Enter - Start Simulation
- Space - Change Theme

### File Menu
- New - Create a new File
- Open - Open a configuration
- Save - Save current configuration (will download a file)

## Tools and Frameworks Used

- C++
- Raylib
- ImGUI
- TinyFileDialogs
- Emscripten