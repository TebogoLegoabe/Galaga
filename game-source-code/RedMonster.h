#ifndef RED_MONSTER_H
#define RED_MONSTER_H

#include "Monster.h"
#include <algorithm>
#include <random>

/**
 * @brief Red monster subclass with more aggressive behavior
 */
class RedMonster : public Monster
{
public:
    /**
     * @brief Constructor for RedMonster
     * @param startPos Starting position in world coordinates
     */
    RedMonster(Vector2 startPos = {0, 0});

    /**
     * @brief Update the red monster
     */
    void update() override;

    /**
     * @brief Draw the red monster
     */
    void draw() override;

    /**
     * @brief Update monster AI to chase the player (RedMonster specific)
     * @param playerPos Player position
     * @param grid Reference to the game grid
     */
    void updateAI(Vector2 playerPos, Grid &grid);

private:
    /**
     * @brief Find the best direction to move toward the player
     * @param playerPos Player position
     * @param grid Reference to the game grid
     * @return Best direction to move
     */
    Direction findBestDirectionToPlayer(Vector2 playerPos, const Grid &grid);

    /**
     * @brief Find a random valid direction to move
     * @param grid Reference to the game grid
     * @return Random valid direction
     */
    Direction findRandomValidDirection(const Grid &grid);
};

#endif // RED_MONSTER_H