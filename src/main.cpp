#include "game-config.hpp"
#include "predeclaration.hpp"
#include "file_manager.hpp"
#include "game-of-life.hpp"

#include <raylib.h>
#include "imgui.h"
#include <rlImGui.h>

#include <chrono>
#include <string>

using std::string;

class GameColorTheme
{
    enum Theme
    {
        DARK,
        LIGHT
    };

public:
    Color background;
    Color foreground;
    Color accent;
    Color text;

    Theme currentTheme = DARK;

    GameColorTheme()
    {
        updateThemeColors();
    }

    void refreshBackground()
    {
        ClearBackground(background);
    }

    void changeTheme()
    {
        currentTheme = (currentTheme == DARK) ? LIGHT : DARK;
        updateThemeColors();
    }

    void updateThemeColors()
    {
        if (currentTheme == DARK)
        {
            background = DARK_THEME_BACKGROUND;
            foreground = DARK_THEME_FOREGROUND;
            accent = DARK_THEME_ACCENT;
            text = DARK_THEME_TEXT;
        }
        else
        {
            background = LIGHT_THEME_BACKGROUND;
            foreground = LIGHT_THEME_FOREGROUND;
            accent = LIGHT_THEME_ACCENT;
            text = LIGHT_THEME_TEXT;
        }
    }
};

class InputManager
{
    KeyboardKey change_color_theme_key = KEY_SPACE;

public:
    InputManager()
    {
        change_color_theme_key = KEY_SPACE;
    }
    void processInput(GameColorTheme &themeManager, ConwayGameWorld &gameWorld)
    {
        if (IsKeyPressed(change_color_theme_key))
        {
            themeManager.changeTheme();
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            gameWorld.isSimulating = !gameWorld.isSimulating;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int x = GetMouseX() / CELL_SIZE;
            int y = (GetMouseY() - BUFFER_HEIGHT) / CELL_SIZE;
            if (y < 0)
            {
                return;
            }
            gameWorld.world[x][y] = !gameWorld.world[x][y];
        }
    }
};

class LinesGenerator
{
public:
    void generateLines(Color lineColor)
    {
        // Draw Horizontal Lines
        for (int i = 0; i < SCREEN_HEIGHT; i += CELL_SIZE)
        {
            DrawLine(0, i + BUFFER_HEIGHT, SCREEN_WIDTH, i + BUFFER_HEIGHT, lineColor);
        }

        // Draw Vertical Lines
        for (int i = 0; i < SCREEN_WIDTH; i += CELL_SIZE)
        {
            DrawLine(i, BUFFER_HEIGHT, i, SCREEN_HEIGHT, lineColor);
        }
    }
};

class GuiManager
{

    ConwayGameWorld *gameWorld;

    bool showControlsScreen = false;
    bool showAboutScreen = false;
    bool showOpenFileBrowser = false;
    bool showSaveAsFileBrowser = false;

public:
    GuiManager(ConwayGameWorld *gameWorld)
    {
        this->gameWorld = gameWorld;
    }

    void render()
    {
        rlImGuiBegin();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                {
                    *gameWorld = ConwayGameWorld();
                }
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    FileDialog fileOpenDialog = FileDialog("Open Game Of Life File", FileDialog::FileDialogType::Open, FILE_EXTENSION);
                    string filePath = fileOpenDialog.OpenDialog();
                    if(filePath != ""){
                        *gameWorld = ConwayGameWorld(FileManager::LoadFromFile(filePath));
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                }
                if (ImGui::MenuItem("Save as.."))
                {
                    FileDialog fileSaveAsDialog = FileDialog("Save as Game Of Life File", FileDialog::FileDialogType::SaveAs, FILE_EXTENSION);
                    string filePath = fileSaveAsDialog.OpenDialog();
                    if(filePath != ""){
                        FileManager::SaveToFile(filePath, gameWorld->generateCurrentSnapShot());
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Controls"))
                {
                    showControlsScreen = true;
                }
                if (ImGui::MenuItem("About"))
                {
                    showAboutScreen = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();

            if (showControlsScreen)
            {
                ImGui::SetNextWindowSize(ImVec2(200, 300));
                ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Controls", &showControlsScreen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ImGui::Text("LMB - Place/Delete Cell");
                ImGui::Text("Enter - Start Simulation");
                ImGui::Text("Space - Change Theme");
                ImGui::End();
            }

            if (showAboutScreen)
            {
                ImGui::SetNextWindowSize(ImVec2(200, 200));
                ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("About", &showAboutScreen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ImGui::Text("Made by GhostOne");
                if (ImGui::Button("Github Repo", ImVec2(50, 20)))
                {
                }
                ImGui::End();
            }
        }
        rlImGuiEnd();
    }
};

int main()
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GhostOne's Conway's Game of Life");
    rlImGuiSetup(true);

    GameColorTheme themeManager = GameColorTheme();
    InputManager inputManager = InputManager();
    LinesGenerator linesGenerator = LinesGenerator();
    ConwayGameWorld gameWorld = ConwayGameWorld();
    GuiManager guiManager = GuiManager(&gameWorld);

    while (!WindowShouldClose())
    {

        inputManager.processInput(themeManager, gameWorld);

        BeginDrawing();
        themeManager.refreshBackground();
        linesGenerator.generateLines(themeManager.accent);
        gameWorld.updateWorld();
        gameWorld.render(themeManager.foreground);
        ;
        guiManager.render();

        EndDrawing();
    }
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}