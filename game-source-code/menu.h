#ifndef MENU_H
#define MENU_H
#include <raylib-cpp.hpp>
#include <string>
#include <vector>

enum class MenuState
{
    MAIN_MENU,
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

    void handleInput();
    /**
     * @brief Draws the menu on the screen.
     */
    void draw();

    MenuState getState() const;
    bool shouldStartGame() const;
    bool shouldExitGame() const;
    void reset();

private:
    MenuState currentState;
    int selectedOption;
    std::vector<std::string> options;
};

#endif // MENU_H