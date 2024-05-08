#pragma once

#include "game-config.hpp"

#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

using std::string, std::getline, std::istringstream;

class ConwayGameWorld
{

    std::chrono::time_point<std::chrono::system_clock> lastSwapTime;

    public:
        bool world[HORIZONTAL_CELL_COUNT][VERTICAL_CELL_COUNT];
        bool nextWorld[HORIZONTAL_CELL_COUNT][VERTICAL_CELL_COUNT];
        bool isSimulating = false;

        ConwayGameWorld();

        ConwayGameWorld(string worldFileContent);

        /// @brief Calculate the next world state based on the current world state
        void calculateNextWorld();

        /// @brief swap between the front and back buffer
        void swapBuffer();

        /// @brief update the current world
        void updateWorld();

        /// @brief render the current world
        void render(Color unitColor);

        /// @brief generate a snapshot of current condition 
        string generateCurrentSnapShot();
    
    private:
        void loadFromString(string worldFileContent);
};