#include "Menu.h"
#include "InputHandler.h"

Menu::Menu()
    : currentState(MenuState::MAIN_MENU),
      selectedOption(0),
      gameWon(false)
{
    mainMenuOptions = {"Start Game", "Instructions", "Exit"};
    currentOptions = mainMenuOptions;
}

void Menu::handleInput()
{
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        handleMainMenuInput();
        break;
    case MenuState::INSTRUCTIONS:
        handleInstructionsInput();
        break;
    case MenuState::GAME_OVER:
    case MenuState::LEVEL_COMPLETE:
        handleGameOverInput();
        break;
    }
}

void Menu::update()
{
    // Any menu animations or updates would go here
}

void Menu::draw()
{
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        drawMainMenu();
        break;
    case MenuState::INSTRUCTIONS:
        drawInstructions();
        break;
    case MenuState::GAME_OVER:
        drawGameOver();
        break;
    case MenuState::LEVEL_COMPLETE:
        drawLevelComplete();
        break;
    }
}

void Menu::handleMainMenuInput()
{
    if (InputHandler::isUpPressed())
    {
        selectedOption = (selectedOption - 1 + currentOptions.size()) % currentOptions.size();
    }
    else if (InputHandler::isDownPressed())
    {
        selectedOption = (selectedOption + 1) % currentOptions.size();
    }
    else if (InputHandler::isEnterPressed())
    {
        switch (selectedOption)
        {
        case 0: // Start Game
            // This will be handled by GameStateManager
            break;
        case 1: // Instructions
            currentState = MenuState::INSTRUCTIONS;
            break;
        case 2: // Exit
            // This will be handled by GameStateManager
            break;
        }
    }
}

void Menu::handleInstructionsInput()
{
    if (InputHandler::isEnterPressed() || InputHandler::isMenuPressed())
    {
        currentState = MenuState::MAIN_MENU;
        selectedOption = 0;
        updateCurrentOptions();
    }
}

void Menu::handleGameOverInput()
{
    if (InputHandler::isEnterPressed())
    {
        reset();
    }
}

void Menu::drawMainMenu()
{
    drawTitle("DIG DUG", 60, ORANGE);

    const char *subtitle = "Underground Adventure";
    int subtitleWidth = MeasureText(subtitle, 30);
    DrawText(subtitle, (GetScreenWidth() - subtitleWidth) / 2,
             GetScreenHeight() / 4 + 70, 30, DARKGRAY);

    drawOptions();
    drawControls();
}

void Menu::drawInstructions()
{
    drawTitle("HOW TO PLAY", 40, ORANGE);

    int screenWidth = GetScreenWidth();
    int startY = GetScreenHeight() / 4 + 50;
    int lineHeight = 25;

    std::vector<std::string> instructions = {
        "- Use ARROW KEYS or WASD to move Dig Dug",
        "- Dig tunnels by moving through the earth",
        "- Press SPACE to shoot your harpoon at monsters",
        "- Monsters move slowly through tunnels and chase you",
        "- Red monsters and green dragons will try to catch you",
        "- Destroy all monsters to win the level!",
        "- Avoid colliding with monsters or you'll lose",
        "",
        "Press ENTER or ESC to return to main menu"};

    for (size_t i = 0; i < instructions.size(); ++i)
    {
        int textWidth = MeasureText(instructions[i].c_str(), 20);
        DrawText(instructions[i].c_str(),
                 (screenWidth - textWidth) / 2,
                 startY + i * lineHeight, 20, DARKGRAY);
    }
}

void Menu::drawGameOver()
{
    if (gameWon)
    {
        drawTitle("LEVEL COMPLETE!", 50, GREEN);
        const char *message = "Congratulations! You defeated all the monsters!";
        int messageWidth = MeasureText(message, 25);
        DrawText(message, (GetScreenWidth() - messageWidth) / 2,
                 GetScreenHeight() / 2, 25, DARKGREEN);
    }
    else
    {
        drawTitle("GAME OVER", 50, RED);
        const char *message = "You were defeated! Try again?";
        int messageWidth = MeasureText(message, 25);
        DrawText(message, (GetScreenWidth() - messageWidth) / 2,
                 GetScreenHeight() / 2, 25, DARKGRAY);
    }

    const char *instruction = "Press ENTER to return to main menu";
    int instructionWidth = MeasureText(instruction, 20);
    DrawText(instruction, (GetScreenWidth() - instructionWidth) / 2,
             GetScreenHeight() / 2 + 50, 20, GRAY);
}

void Menu::drawLevelComplete()
{
    drawGameOver(); // Same as game over but with won = true
}

void Menu::drawTitle(const char *title, int fontSize, Color color)
{
    int titleWidth = MeasureText(title, fontSize);
    DrawText(title, (GetScreenWidth() - titleWidth) / 2,
             GetScreenHeight() / 4, fontSize, color);
}

void Menu::drawOptions()
{
    int startY = GetScreenHeight() / 2;

    for (size_t i = 0; i < currentOptions.size(); ++i)
    {
        Color color = (i == selectedOption) ? ORANGE : DARKGRAY;
        int textWidth = MeasureText(currentOptions[i].c_str(), 25);
        DrawText(currentOptions[i].c_str(),
                 (GetScreenWidth() - textWidth) / 2,
                 startY + i * 40, 25, color);
    }
}

void Menu::drawControls()
{
    const char *controls = "Use UP/DOWN arrows to navigate, ENTER to select";
    int controlsWidth = MeasureText(controls, 18);
    DrawText(controls, (GetScreenWidth() - controlsWidth) / 2,
             GetScreenHeight() - 50, 18, GRAY);
}

void Menu::updateCurrentOptions()
{
    switch (currentState)
    {
    case MenuState::MAIN_MENU:
        currentOptions = mainMenuOptions;
        break;
    default:
        currentOptions = mainMenuOptions;
        break;
    }
}

MenuState Menu::getMenuState() const
{
    return currentState;
}

bool Menu::shouldStartGame() const
{
    return currentState == MenuState::MAIN_MENU &&
           selectedOption == 0 &&
           InputHandler::isEnterPressed();
}

bool Menu::shouldExitGame() const
{
    return currentState == MenuState::MAIN_MENU &&
           selectedOption == 2 &&
           InputHandler::isEnterPressed();
}

void Menu::reset()
{
    currentState = MenuState::MAIN_MENU;
    selectedOption = 0;
    gameWon = false;
    updateCurrentOptions();
}

void Menu::setGameOver(bool playerWon)
{
    currentState = MenuState::GAME_OVER;
    gameWon = playerWon;
}

void Menu::setLevelComplete()
{
    currentState = MenuState::LEVEL_COMPLETE;
    gameWon = true;
}