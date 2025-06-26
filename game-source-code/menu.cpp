#include "menu.h"
#include <vector>
#include <string>

Menu::Menu() : currentState(MenuState::MAIN_MENU), selectedOption(0), volumeLevel(50), fullscreen(false), difficulty(1)
{
    mainMenuOptions = {"Start Game", "Settings", "Instructions", "Exit"};
    settingsOptions = {"Volume", "Fullscreen", "Difficulty", "Back to Main Menu"};

    currentOptions = mainMenuOptions; // Start with main menu options
}

void Menu::handleInput()
{
    // handle input for menu navigation
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        handleMainMenuInput();
        break;
    case MenuState::SETTINGS:
        handleSettingsInput();
        break;
    case MenuState::INSTRUCTIONS:
        handleInstructionsInput();
        break;
    default:
        break;
    }
}

void Menu::handleMainMenuInput()
{
    if (IsKeyPressed(KEY_UP))
    {
        selectedOption = (selectedOption - 1 + currentOptions.size()) % currentOptions.size();
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption = (selectedOption + 1) % currentOptions.size();
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        switch (selectedOption)
        {
        case 0: // Start Game
            currentState = MenuState::PLAYING;
            break;
        case 1: // Settings
            currentState = MenuState::SETTINGS;
            selectedOption = 0; // Reset selected option for settings
            break;
        case 2: // Instructions
            currentState = MenuState::INSTRUCTIONS;
            break;
        case 3: // Exit
            currentState = MenuState::EXIT;
            break;
        }
    }
}

void Menu::handleSettingsInput()
{
    if (IsKeyPressed(KEY_UP))
    {
        selectedOption = (selectedOption - 1 + settingsOptions.size()) % settingsOptions.size();
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption = (selectedOption + 1) % settingsOptions.size();
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        switch (selectedOption)
        {
        case 0:                                     // Volume
            volumeLevel = (volumeLevel + 25) % 110; // Cycle through volume levels
            if (volumeLevel == 100)
                volumeLevel = 0; // Reset to 0 after 100
            break;
        case 1: // Fullscreen
            fullscreen = !fullscreen;
            settingsOptions[1] = "Fullscreen: " + std::string(fullscreen ? "On" : "Off");
            break;
        case 2:                                // Difficulty
            difficulty = (difficulty + 1) % 3; // Cycle through difficulties: 0, 1, 2
            settingsOptions[2] = "Difficulty: " + std::to_string(difficulty);
            break;
        case 3: // Back to Main Menu
            currentState = MenuState::MAIN_MENU;
            selectedOption = 0; // Reset selected option for main menu
            break;
        }
    }
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        currentState = MenuState::MAIN_MENU; // Go back to main menu
        selectedOption = 0;                  // Reset selected option for main menu
        updateCurrentOptions();              // Update current options to main menu
    }
}

void Menu::handleInstructionsInput()
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
    {
        currentState = MenuState::MAIN_MENU;
        selectedOption = 0;
        updateCurrentOptions();
    }
}

void Menu::updateCurrentOptions()
{
    // Update the current options based on the current state
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        currentOptions = mainMenuOptions;
        break;
    case MenuState::SETTINGS:
        currentOptions = settingsOptions;
        break;
    default:
        currentOptions = mainMenuOptions;
        break;
    }
}
void Menu::draw()
{
    // clear the background
    ClearBackground(RAYWHITE);

    // Draw the current menu based on the state
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        drawMainMenu();
        break;
    case MenuState::SETTINGS:
        drawSettings();
        break;
    case MenuState::INSTRUCTIONS:
        drawInstructions();
        break;
    default:
        drawMainMenu();
    }
}

void Menu::drawMainMenu()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw the main menu options
    const char *title = "GALAGA GAME";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 4, 60, DARKBLUE);

    const char *subtitle = "Main Menu";
    int subtitleWidth = MeasureText(subtitle, 30);
    DrawText(subtitle, (screenWidth - subtitleWidth) / 2, screenHeight / 4 + 50, 30, DARKGRAY);

    // Draw menu options
    for (size_t i = 0; i < currentOptions.size(); ++i)
    {
        Color color = (i == selectedOption) ? DARKBLUE : DARKGRAY; // Highlight selected option
        int textWidth = MeasureText(currentOptions[i].c_str(), 20);
        DrawText(currentOptions[i].c_str(), (screenWidth - textWidth) / 2, screenHeight / 2 + i * 30, 20, color);
    }
    // Draw instructions
    const char *instructions = "Use UP/DOWN to navigate, ENTER to select, ESC to go back.";
    int instructionsWidth = MeasureText(instructions, 25);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, screenHeight - 50, 25, DARKGRAY);
}

void Menu::drawSettings()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw the settings options
    const char *title = "Settings";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 4, 40, DARKBLUE);

    // Draw settings options
    for (size_t i = 0; i < currentOptions.size(); ++i)
    {
        Color color = (i == selectedOption) ? DARKBLUE : DARKGRAY; // Highlight selected option
        int textWidth = MeasureText(currentOptions[i].c_str(), 20);
        DrawText(currentOptions[i].c_str(), (screenWidth - textWidth) / 2, screenHeight / 2 + i * 30, 20, color);
    }

    // Draw instructions
    const char *instructions = "Use UP/DOWN to navigate, ENTER to select, ESC to go back.";
    int instructionsWidth = MeasureText(instructions, 15);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, screenHeight - 50, 15, DARKGRAY);
}

void Menu::drawInstructions()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw instructions title
    const char *title = "HOW TO PLAY";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, (screenWidth - titleWidth) / 2, 50, 40, DARKBLUE);
    // Draw instructions text
    const char *instructions = "Use arrow keys to move your ship.\n"
                               "Press SPACE to shoot.\n"
                               "Avoid enemy ships and their bullets.\n"
                               "Collect power-ups to enhance your ship.\n"
                               "Defeat the boss to win the game!";
    int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, screenHeight / 4, 20, DARKGRAY);
}

MenuState Menu::getState() const
{
    return currentState;
}
bool Menu::shouldStartGame() const
{
    return currentState == MenuState::PLAYING;
}
bool Menu::shouldExitGame() const
{
    return currentState == MenuState::EXIT;
}
void Menu::reset()
{
    currentState = MenuState::MAIN_MENU;
    selectedOption = 0;
    updateCurrentOptions();
}
