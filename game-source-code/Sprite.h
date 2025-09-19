#ifndef SPRITE_H
#define SPRITE_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"

/**
 * @brief Simple sprite rendering system for game characters
 */
class Sprite
{
public:
    /**
     * @brief Draw Dig Dug player sprite
     * @param position Position to draw at
     * @param size Size of the sprite
     * @param direction Direction the character is facing
     */
    static void drawDigDug(Vector2 position, Vector2 size, Direction direction);

    /**
     * @brief Draw a rock sprite
     * @param position Position to draw at
     * @param size Size of the sprite
     */
    static void drawRock(Vector2 position, Vector2 size);

private:
    /**
     * @brief Draw Dig Dug facing right
     * @param pos Position to draw at
     * @param size Size of the sprite
     */
    static void drawDigDugRight(Vector2 pos, Vector2 size);

    /**
     * @brief Draw Dig Dug facing left
     * @param pos Position to draw at
     * @param size Size of the sprite
     */
    static void drawDigDugLeft(Vector2 pos, Vector2 size);

    /**
     * @brief Draw Dig Dug facing up
     * @param pos Position to draw at
     * @param size Size of the sprite
     */
    static void drawDigDugUp(Vector2 pos, Vector2 size);

    /**
     * @brief Draw Dig Dug facing down
     * @param pos Position to draw at
     * @param size Size of the sprite
     */
    static void drawDigDugDown(Vector2 pos, Vector2 size);
};

#endif // SPRITE_H