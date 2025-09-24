#include "InputHandler.h"

Direction InputHandler::getDirectionInput()
{
    // Changed from IsKeyPressed to IsKeyDown to detect held keys
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        return Direction::UP;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        return Direction::DOWN;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        return Direction::LEFT;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        return Direction::RIGHT;

    return Direction::NONE;
}

bool InputHandler::isDirectionPressed(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    case Direction::DOWN:
        return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    case Direction::LEFT:
        return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    case Direction::RIGHT:
        return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    default:
        return false;
    }
}

bool InputHandler::isActionPressed()
{
    return IsKeyPressed(KEY_SPACE); // Keep as pressed for shooting
}

bool InputHandler::isMenuPressed()
{
    return IsKeyPressed(KEY_ESCAPE); // Keep as pressed for menu
}

bool InputHandler::isUpPressed()
{
    return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W); // Keep as pressed for menu navigation
}

bool InputHandler::isDownPressed()
{
    return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S); // Keep as pressed for menu navigation
}

bool InputHandler::isEnterPressed()
{
    return IsKeyPressed(KEY_ENTER); // Keep as pressed for menu selection
}