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

static char filename[100] = "";

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
            if(gameWorld.isInteractable){
                int x = GetMouseX() / CELL_SIZE;
                int y = (GetMouseY() - BUFFER_HEIGHT) / CELL_SIZE;
                if (y < 0)
                {
                    return;
                }
                gameWorld.world[x][y] = !gameWorld.world[x][y];
            }
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
    bool showSaveAsFileBrowser = false;
public:
    GuiManager(ConwayGameWorld *gameWorld)
    {
        this->gameWorld = gameWorld;
    }

    void render()
    {
        gameWorld->isInteractable = true;
        rlImGuiBegin();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {   
                gameWorld->isInteractable = false;
                if (ImGui::MenuItem("New"))
                {
                    *gameWorld = ConwayGameWorld();
                }
                if (ImGui::MenuItem("Open"))
                {
                    FileDialog fileOpenDialog = FileDialog("Open Game Of Life File", FileDialog::FileDialogType::Open, FILE_PATTERN, FILE_EXT);
                    string filePath = fileOpenDialog.OpenDialog();
                    if(filePath != ""){
                        #ifdef __EMSCRIPTEN__
                        gfle = FileLoadCompletionEvent([filePath, this](){
                            *this->gameWorld = ConwayGameWorld(FileManager::LoadFromFile(filePath));
                        }); 
                        #else
                        *gameWorld = ConwayGameWorld(FileManager::LoadFromFile(filePath));
                        #endif
                    }
                }
                if (ImGui::MenuItem("Save"))
                {
                    #ifdef __EMSCRIPTEN__
                    showSaveAsFileBrowser = true;
                    #else
                    FileDialog fileSaveAsDialog = FileDialog("Save as Game Of Life File", FileDialog::FileDialogType::SaveAs, FILE_PATTERN, FILE_EXT);
                    string filePath = fileSaveAsDialog.OpenDialog();
                    if(filePath != ""){
                        FileManager::SaveToFile(filePath, gameWorld->generateCurrentSnapShot());
                    }
                    #endif
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                gameWorld->isInteractable = false;
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
                gameWorld->isInteractable = false;
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
                gameWorld->isInteractable = false;
            }

            if(showSaveAsFileBrowser){
                ImGui::SetNextWindowSize(ImVec2(200, 200));
                ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Download Config", &showSaveAsFileBrowser, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
                ImGui::InputText("Name", filename, IM_ARRAYSIZE(filename));
                if (ImGui::Button("Download", ImVec2(100, 20)))
                {
                    //std::cout << gameWorld -> generateCurrentSnapShot() << std::endl;
                    FileManager::DownloadFileMEMFSToDisk(filename, FILE_EXT, gameWorld->generateCurrentSnapShot());
                    showSaveAsFileBrowser = false;
                }
                ImGui::End();
                gameWorld->isInteractable = false;
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