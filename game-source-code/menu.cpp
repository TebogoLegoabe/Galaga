#include "menu.h"

Menu::Menu() : currentState(MenuState::MAIN_MENU), selectedOption(0)
{
    // options = {"Start Game", "Exit"};
    options.push_back("Start Game");
    options.push_back("Exit");
}

void Menu::handleInput()
{
    // handle input for menu navigation
    if (IsKeyPressed(KEY_UP))
    {
        selectedOption = (selectedOption - 1 + options.size()) % options.size();
    }
    else if (IsKeyPressed(KEY_DOWN))
    {
        selectedOption = (selectedOption + 1) % options.size();
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        if (selectedOption == 0)
        {
            currentState = MenuState::PLAYING; // Start game
        }
        else if (selectedOption == 1)
        {
            currentState = MenuState::EXIT; // Exit game
        }
    }
}

void Menu::draw()
{
    // Draw the mnenu on the screen
    ClearBackground(RAYWHITE);

    // calculate the position to center the menu
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Draw menu title
    const char *title = "Galaga Game";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 4, 40, BLACK);

    const char *subtitle = "Main Menu";
    int subtitleWidth = MeasureText(subtitle, 20);
    DrawText(subtitle, (screenWidth - subtitleWidth) / 2, screenHeight / 4 + 50, 20, DARKGRAY);

    // Draw menu options
    int startY = screenHeight / 2;
    for (size_t i = 0; i < options.size(); ++i)
    {
        Color color = (i == selectedOption) ? RED : DARKGRAY; // Highlight selected option
        int textWidth = MeasureText(options[i].c_str(), 20);
        DrawText(options[i].c_str(), (screenWidth - textWidth) / 2, startY + i * 30, 20, color);

        // Draw a rectangle around the selected option
        /* if (i == selectedOption)
          {
              int rectWidth = textWidth + 20; // Add padding
              DrawRectangle((screenWidth - rectWidth) / 2 - 10, startY + i * 30 - 5, rectWidth, 30, Fade(RED, 0.3f));
          } */
        if (i == selectedOption)
        {
            DrawText(">", (screenWidth - textWidth) / 2 - 30, startY + i * 50, 24, RED);
        }
    }

    // Draw instructions
    const char *instructions = "Use UP/DOWN to navigate, ENTER to select";
    int instructionsWidth = MeasureText(instructions, 20);
    DrawText(instructions, (screenWidth - instructionsWidth) / 2, screenHeight - 50, 20, DARKGRAY);
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
}