#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include <raylib.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 620
#define BUFFER_HEIGHT 20
#define CELL_SIZE 10
#define SWAP_FREQUENCY 50

#define DARK_THEME_BACKGROUND (Color){ 0, 0, 0, 0xff }
#define DARK_THEME_ACCENT GRAY
#define DARK_THEME_FOREGROUND (Color){ 0xff, 0xff, 0xff, 0xff }
#define DARK_THEME_TEXT (Color){0x80, 0x80, 0x80, 0xff}

#define LIGHT_THEME_BACKGROUND RAYWHITE
#define LIGHT_THEME_ACCENT GRAY
#define LIGHT_THEME_FOREGROUND BLACK
#define LIGHT_THEME_TEXT (Color){0x80, 0x80, 0x80, 0xff}

#endif