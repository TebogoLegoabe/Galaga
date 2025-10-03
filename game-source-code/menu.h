#ifndef MENU_H
#define MENU_H

#include <raylib-cpp.hpp>
#include <string>
#include <vector>
#include "GameEnums.h"

/**
 * @brief Class representing the game menu system
 */
class Menu
{
public:
    /**
     * @brief Default constructor for the Menu class
     */
    Menu();

    /**
     * @brief Handle user input for menu navigation
     */
    void handleInput();

    /**
     * @brief Update menu state
     */
    void update();

    /**
     * @brief Draw the menu on the screen
     */
    void draw();

    /**
     * @brief Get the current menu state
     * @return The current MenuState
     */
    MenuState getMenuState() const;

    /**
     * @brief Check if the game should start
     * @return true if the game should start, false otherwise
     */
    bool shouldStartGame() const;

    /**
     * @brief Check if the game should exit
     * @return true if the game should exit, false otherwise
     */
    bool shouldExitGame() const;

    /**
     * @brief Reset the menu to its initial state
     */
    void reset();

    /**
     * @brief Set the menu to game over state
     * @param playerWon true if player won, false if lost
     */
    void setGameOver(bool playerWon);

    /**
     * @brief Set the menu to level complete state
     */
    void setLevelComplete();

private:
    MenuState currentState;                   // Current menu state
    int selectedOption;                       // Currently selected menu option
    std::vector<std::string> mainMenuOptions; // Main menu options
    std::vector<std::string> currentOptions;  // Current displayed options
    bool gameWon;                             // Whether the player won the last game

    // Private helper methods
    void drawMainMenu();
    void drawInstructions();
    void drawGameOver();
    void drawLevelComplete();
    void handleMainMenuInput();
    void handleInstructionsInput();
    void handleGameOverInput();
    void updateCurrentOptions();

    // Helper methods for drawing
    void drawTitle(const char *title, int fontSize, Color color);
    void drawOptions();
    void drawControls();
};

#endif // MENU_H