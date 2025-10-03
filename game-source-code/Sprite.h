#ifndef SPRITE_H
#define SPRITE_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"

/**
 * @brief Sprite class for drawing characters and objects
 */
class Sprite
{
public:
    /**
     * @brief Constructor for Sprite
     * @param pos Position to draw the sprite
     * @param spriteSize Size of the sprite
     */
    Sprite(Vector2 pos = {0, 0}, Vector2 spriteSize = {32, 32});

    /**
     * @brief Draw Dig Dug character
     * @param position Position to draw at
     * @param direction Direction the character is facing
     * @param size Size to draw the sprite
     */
    static void drawDigDug(Vector2 position, Direction direction, Vector2 size);

    /**
     * @brief Draw red monster
     * @param position Position to draw at
     * @param size Size to draw the sprite
     */
    static void drawRedMonster(Vector2 position, Vector2 size);

    /**
     * @brief Draw green dragon
     * @param position Position to draw at
     * @param size Size to draw the sprite
     */
    static void drawGreenDragon(Vector2 position, Vector2 size);

    /**
     * @brief Draw disembodied eyes (when monster is floating)
     * @param position Position to draw at
     * @param size Size to draw the sprite
     * @param isGreen Whether this is from a green dragon
     */
    static void drawDisembodiedEyes(Vector2 position, Vector2 size, bool isGreen = false);

    /**
     * @brief Draw harpoon projectile
     * @param position Position to draw at
     * @param direction Direction the harpoon is traveling
     * @param size Size to draw the sprite
     */
    static void drawHarpoon(Vector2 position, Direction direction, Vector2 size);

    /**
     * @brief Draw fire projectile
     * @param position Position to draw at
     * @param direction Direction the fire is traveling
     * @param size Size to draw the sprite
     * @param intensity Fire intensity (0.0 to 1.0)
     * @param animationOffset Animation offset for flickering
     */
    static void drawFire(Vector2 position, Direction direction, Vector2 size, float intensity = 1.0f, float animationOffset = 0.0f);

    /**
     * @brief Draw rock
     * @param position Position to draw at
     * @param size Size to draw the sprite
     */
    static void drawRock(Vector2 position, Vector2 size);

    /**
     * @brief Draw a simple character sprite with customizable colors
     * @param position Position to draw at
     * @param size Size of the sprite
     * @param bodyColor Main body color
     * @param accentColor Accent/detail color
     * @param direction Direction facing (for asymmetric sprites)
     */
    static void drawCharacterSprite(Vector2 position, Vector2 size, Color bodyColor, Color accentColor, Direction direction = Direction::RIGHT);

private:
    Vector2 position;
    Vector2 size;

    /**
     * @brief Draw eyes on a character
     * @param center Center position of the character
     * @param eyeSize Size of the eyes
     * @param eyeColor Color of the eyes
     */
    static void drawEyes(Vector2 center, float eyeSize, Color eyeColor = WHITE);

    /**
     * @brief Draw direction indicator
     * @param center Center position
     * @param direction Direction to indicate
     * @param indicatorSize Size of the indicator
     * @param color Color of the indicator
     */
    static void drawDirectionIndicator(Vector2 center, Direction direction, float indicatorSize, Color color = WHITE);
};

#endif // SPRITE_H