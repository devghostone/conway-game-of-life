#include "game-config.hpp"
#include "predeclaration.h"

#include <raylib.h>
#include <chrono>

const int HORIZONTAL_CELL_COUNT = (SCREEN_WIDTH/CELL_SIZE);
const int VERTICAL_CELL_COUNT = (SCREEN_HEIGHT/CELL_SIZE);

using namespace std;

class ConwayGameWorld{

    std::chrono::time_point<std::chrono::system_clock> lastSwapTime;

    public:
        bool world[HORIZONTAL_CELL_COUNT][VERTICAL_CELL_COUNT];
        bool nextWorld[HORIZONTAL_CELL_COUNT][VERTICAL_CELL_COUNT];
        bool isSimulating = false;

        ConwayGameWorld(){

            lastSwapTime = std::chrono::system_clock::now();
            isSimulating = false;

            for(int i = 0; i < HORIZONTAL_CELL_COUNT; i++){
                for(int j = 0; j < VERTICAL_CELL_COUNT; j++){
                    world[i][j] = false;
                    nextWorld[i][j] = false;
                }
            }
        }

        /// @brief Calculate the next world state based on the current world state
        void calculateNextWorld(){
            for(int i = 0; i < HORIZONTAL_CELL_COUNT; i++){
                for(int j = 0; j < VERTICAL_CELL_COUNT; j++){
                    /// - Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                    /// - Any live cell with two or three live neighbors lives on to the next generation.
                    /// - Any live cell with more than three live neighbors dies, as if by overpopulation.
                    /// - Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

                    bool isCurrentCellAlive = world[i][j];
                    int livingNeighbours = 0;

                    for(int x = -1; x <= 1; x++){
                        for (int y = -1; y <= 1; y++){
                            int actualX = ((i + x) + HORIZONTAL_CELL_COUNT) % (HORIZONTAL_CELL_COUNT);
                            int actualY = ((j + y) + VERTICAL_CELL_COUNT) % VERTICAL_CELL_COUNT;
                            if(i == actualX && j == actualY){
                                continue;
                            }
                            if(world[actualX][actualY]){
                                livingNeighbours++;
                            }
                        }
                    }

                    if(!isCurrentCellAlive){
                        /// - Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
                        if(livingNeighbours == 3){
                            nextWorld[i][j] = true;
                        }
                        continue;
                    }

                    if(livingNeighbours < 2 || livingNeighbours > 3){
                        /// - Any live cell with fewer than two live neighbors dies, as if by underpopulation.
                        nextWorld[i][j] = false;
                        continue;
                    }
                    if(livingNeighbours == 2 || livingNeighbours == 3){
                        /// - Any live cell with two or three live neighbors lives on to the next generation
                        nextWorld[i][j] = true;
                        continue;
                    }
                }
            }
        }

        void swapBuffer(){
            for(int i = 0; i < HORIZONTAL_CELL_COUNT; i++){
                for(int j = 0; j < VERTICAL_CELL_COUNT; j++){
                    world[i][j] = nextWorld[i][j];
                    nextWorld[i][j] = false;
                }
            }
        }

        void updateWorld(){
            if(isSimulating){
                if(lastSwapTime + std::chrono::milliseconds(SWAP_FREQUENCY) < std::chrono::system_clock::now()){
                    calculateNextWorld();
                    swapBuffer();
                    lastSwapTime = std::chrono::system_clock::now();
                }
            }
        }

        void render(Color unitColor){
            for(int i = 0; i < HORIZONTAL_CELL_COUNT; i++){
                for(int j = 0; j < VERTICAL_CELL_COUNT; j++){
                    if(world[i][j]){
                        DrawRectangle(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE, unitColor);
                    }
                }
            }
        }
};

class GameColorTheme{
    enum Theme{
        DARK,
        LIGHT
    };
    public:

        Color background;
        Color foreground;
        Color accent;
        Color text;

        Theme currentTheme = DARK;

        GameColorTheme(){
            updateThemeColors();
        }

        void refreshBackground(){
            ClearBackground(background);
        }

        void changeTheme(){
            currentTheme = (currentTheme == DARK) ? LIGHT : DARK;
            updateThemeColors();
        }

        void updateThemeColors(){
            if(currentTheme == DARK){
                background = DARK_THEME_BACKGROUND;
                foreground = DARK_THEME_FOREGROUND;
                accent = DARK_THEME_ACCENT;
                text = DARK_THEME_TEXT;
            }else{
                background = LIGHT_THEME_BACKGROUND;
                foreground = LIGHT_THEME_FOREGROUND;
                accent = LIGHT_THEME_ACCENT;
                text = LIGHT_THEME_TEXT;
            }
        }
};

class InputManager{
    KeyboardKey change_color_theme_key = KEY_SPACE;

    public:

        InputManager(){
            change_color_theme_key = KEY_SPACE;
        }
        void processInput(GameColorTheme& themeManager, ConwayGameWorld& gameWorld){
            if(IsKeyPressed(change_color_theme_key)){
                themeManager.changeTheme();
            }

            if(IsKeyPressed(KEY_ENTER)){
                gameWorld.isSimulating = !gameWorld.isSimulating;
            }

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                int x = GetMouseX()/CELL_SIZE;
                int y = GetMouseY()/CELL_SIZE;
                gameWorld.world[x][y] = !gameWorld.world[x][y];
            }
        }
};

class LinesGenerator{
    public:
        void generateLines(Color lineColor){
            // Draw Horizontal Lines
            for(int i = 0; i < SCREEN_HEIGHT; i += CELL_SIZE){
                DrawLine(0, i, SCREEN_WIDTH, i, lineColor);
            }

            // Draw Vertical Lines
            for(int i = 0; i < SCREEN_WIDTH; i += CELL_SIZE){
                DrawLine(i, 0, i, SCREEN_HEIGHT, lineColor);
            }
        }
};

int main(){
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GhostOne's Conway's Game of Life");

    GameColorTheme themeManager = GameColorTheme();
    InputManager inputManager = InputManager();
    LinesGenerator linesGenerator = LinesGenerator();
    ConwayGameWorld gameWorld = ConwayGameWorld();

    while(!WindowShouldClose()){

        inputManager.processInput(themeManager, gameWorld);

        BeginDrawing();
        themeManager.refreshBackground();
        linesGenerator.generateLines(themeManager.accent);
        gameWorld.updateWorld();
        gameWorld.render(themeManager.foreground);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}