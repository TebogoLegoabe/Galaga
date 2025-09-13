#include "InputHandler.h"

Direction InputHandler::getDirectionInput()
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        return Direction::UP;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        return Direction::DOWN;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        return Direction::LEFT;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        return Direction::RIGHT;

    return Direction::NONE;
}

bool InputHandler::isDirectionPressed(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
    case Direction::DOWN:
        return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
    case Direction::LEFT:
        return IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    case Direction::RIGHT:
        return IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
    default:
        return false;
    }
}

bool InputHandler::isActionPressed()
{
    return IsKeyPressed(KEY_SPACE);
}

bool InputHandler::isMenuPressed()
{
    return IsKeyPressed(KEY_ESCAPE);
}

bool InputHandler::isUpPressed()
{
    return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
}

bool InputHandler::isDownPressed()
{
    return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
}

bool InputHandler::isEnterPressed()
{
    return IsKeyPressed(KEY_ENTER);
}