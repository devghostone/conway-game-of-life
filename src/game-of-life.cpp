#include "game-of-life.hpp"

ConwayGameWorld::ConwayGameWorld()
{
    lastSwapTime = std::chrono::system_clock::now();
    isSimulating = false;

    for (int i = 0; i < HORIZONTAL_CELL_COUNT; i++)
    {
        for (int j = 0; j < VERTICAL_CELL_COUNT; j++)
        {
            world[i][j] = false;
            nextWorld[i][j] = false;
        }
    }
}

ConwayGameWorld::ConwayGameWorld(string worldFileContent)
{
    *this = ConwayGameWorld();
    loadFromString(worldFileContent);
}

void ConwayGameWorld::calculateNextWorld()
{
    for (int i = 0; i < HORIZONTAL_CELL_COUNT; i++)
    {
        for (int j = 0; j < VERTICAL_CELL_COUNT; j++)
        {
            /// - Any live cell with fewer than two live neighbors dies, as if by underpopulation.
            /// - Any live cell with two or three live neighbors lives on to the next generation.
            /// - Any live cell with more than three live neighbors dies, as if by overpopulation.
            /// - Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

            bool isCurrentCellAlive = world[i][j];
            int livingNeighbours = 0;

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int actualX = ((i + x) + HORIZONTAL_CELL_COUNT) % (HORIZONTAL_CELL_COUNT);
                    int actualY = ((j + y) + VERTICAL_CELL_COUNT) % VERTICAL_CELL_COUNT;
                    if (i == actualX && j == actualY)
                    {
                        continue;
                    }
                    if (world[actualX][actualY])
                    {
                        livingNeighbours++;
                    }
                }
            }

            if (!isCurrentCellAlive)
            {
                /// - Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
                if (livingNeighbours == 3)
                {
                    nextWorld[i][j] = true;
                }
                continue;
            }

            if (livingNeighbours < 2 || livingNeighbours > 3)
            {
                /// - Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                nextWorld[i][j] = false;
                continue;
            }
            if (livingNeighbours == 2 || livingNeighbours == 3)
            {
                /// - Any live cell with two or three live neighbors lives on to the next generation
                nextWorld[i][j] = true;
                continue;
            }
        }
    }
}

void ConwayGameWorld::swapBuffer()
{
    for (int i = 0; i < HORIZONTAL_CELL_COUNT; i++)
    {
        for (int j = 0; j < VERTICAL_CELL_COUNT; j++)
        {
            world[i][j] = nextWorld[i][j];
            nextWorld[i][j] = false;
        }
    }
}

void ConwayGameWorld::updateWorld()
{
    if (isSimulating)
    {
        if (lastSwapTime + std::chrono::milliseconds(SWAP_FREQUENCY) < std::chrono::system_clock::now())
        {
            calculateNextWorld();
            swapBuffer();
            lastSwapTime = std::chrono::system_clock::now();
        }
    }
}

void ConwayGameWorld::render(Color unitColor)
{
    for (int i = 0; i < HORIZONTAL_CELL_COUNT; i++)
    {
        for (int j = 0; j < VERTICAL_CELL_COUNT; j++)
        {
            if (world[i][j])
            {
                DrawRectangleRounded((Rectangle){static_cast<float>(i * CELL_SIZE), static_cast<float>((j * CELL_SIZE) + BUFFER_HEIGHT), CELL_SIZE, CELL_SIZE}, 50, 10, unitColor);
                //DrawRectangle(i * CELL_SIZE, (j * CELL_SIZE) + BUFFER_HEIGHT, CELL_SIZE, CELL_SIZE, unitColor);
            }
        }
    }
}

string ConwayGameWorld::generateCurrentSnapShot()
{
    string worldSnapshot;
    for (int i = 0; i < HORIZONTAL_CELL_COUNT; i++)
    {
        for (int j = 0; j < VERTICAL_CELL_COUNT; j++)
        {
            if (world[i][j])
            {
                char buffer[7];
                sprintf(buffer, "%d,%d\n", i, j);
                worldSnapshot.append(buffer);
            }
        }
    }
    return worldSnapshot;
}

void ConwayGameWorld::loadFromString(string worldFileContent){
    istringstream ss(worldFileContent);
    string line;

    while(getline(ss, line, '\n')){
        istringstream inner_ss(line);
        string content;
        bool isX = true;
        int X = -1;
        int Y = -1;
        while(getline(inner_ss, content, ',')){
            if(isX){
                X = std::stoi(content);
                isX = false;
            }else{
                Y = std::stoi(content);
            }
        }
        world[X][Y] = true;
    }
}