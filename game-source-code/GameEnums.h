#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

/**
 * @brief Enumeration for game states
 */
enum class GameState
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE
};

/**
 * @brief Enumeration for movement directions
 */
enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @brief Enumeration for tile types in the game grid
 */
enum class TileType
{
    EARTH,
    TUNNEL,
    ROCK
};

/**
 * @brief Enumeration for monster states
 */
enum class MonsterState
{
    IN_TUNNEL,
    DISEMBODIED,
    DEAD
};

/**
 * @brief Enumeration for menu states
 */
enum class MenuState
{
    MAIN_MENU,
    INSTRUCTIONS,
    GAME_OVER,
    LEVEL_COMPLETE
};

#endif // GAME_ENUMS_H
