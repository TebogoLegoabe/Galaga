#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"

/**
 * @brief Handles all input for the game
 */
class InputHandler
{
public:
    /**
     * @brief Get the direction input from the player
     * @return Direction enum value
     */
    static Direction getDirectionInput();

    /**
     * @brief Check if a specific direction key is pressed
     * @param dir Direction to check
     * @return true if the direction key is pressed
     */
    static bool isDirectionPressed(Direction dir);

    /**
     * @brief Check if the action key (space/enter) is pressed
     * @return true if action key is pressed
     */
    static bool isActionPressed();

    /**
     * @brief Check if the menu key (escape) is pressed
     * @return true if menu key is pressed
     */
    static bool isMenuPressed();

    /**
     * @brief Check if the up navigation key is pressed (for menus)
     * @return true if up key is pressed
     */
    static bool isUpPressed();

    /**
     * @brief Check if the down navigation key is pressed (for menus)
     * @return true if down key is pressed
     */
    static bool isDownPressed();

    /**
     * @brief Check if the enter key is pressed
     * @return true if enter key is pressed
     */
    static bool isEnterPressed();
};

#endif // INPUT_HANDLER_H