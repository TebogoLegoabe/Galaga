#ifndef MENU_H
#define MENU_H
#include <raylib-cpp.hpp>
#include <string>
#include <vector>

enum class MenuState
{
    MAIN_MENU,
    SETTINGS,
    INSTRUCTIONS,
    PLAYING,
    EXIT
};

/**
 * @brief Class representing the game menu.
 */

class Menu
{
public:
    /**
     *  @brief Default constructor for the Menu class.
     */
    Menu();
    /**
     * @brief Handles user input for menu navigation.
     * This method allows the user to navigate through the menu options
     * and select an option to start the game or exit.
     */
    void handleInput();
    /**
     * @brief Draws the menu on the screen.
     */
    void draw();
    /**
     * @brief Gets the current state of the menu.
     * @return The current MenuState.
     */
    MenuState getState() const;
    /**
     * @brief Checks if the game should start.
     * @return true if the game should start, false otherwise.
     */
    bool shouldStartGame() const;
    /**
     * @brief Checks if the game should exit.
     * @return true if the game should exit, false otherwise.
     */
    bool shouldExitGame() const;
    /**
     * @brief Resets the menu to its initial state.
     * This method resets the selected option and current state of the menu.
     */
    void reset();

private:
    MenuState currentState;
    int selectedOption;
    std::vector<std::string> mainMenuOptions;
    std::vector<std::string> settingsOptions;
    std::vector<std::string> currentOptions;

    // private helper methods
    void drawMainMenu();
    void drawSettings();
    void drawInstructions();
    void handleMainMenuInput();
    void handleSettingsInput();
    void handleInstructionsInput();
    void updateCurrentOptions();

    // settings variables
    int volumeLevel; // Volume level for the game
    bool fullscreen; // Fullscreen mode toggle
    int difficulty;  // 0: Easy, 1: Medium, 2: Hard
};

#endif // MENU_H