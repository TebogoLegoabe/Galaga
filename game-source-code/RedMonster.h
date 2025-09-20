#ifndef RED_MONSTER_H
#define RED_MONSTER_H

#include "Monster.h"

/**
 * @brief Red monster class - specific type of monster
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
     * @brief Update red monster AI and movement
     * @param playerPos Player's current position
     * @param grid Reference to the game grid
     */
    void updateAI(Vector2 playerPos, Grid &grid);

private:
    /**
     * @brief Choose next direction for red monster movement
     * @param grid Reference to the game grid
     * @return Direction to move
     */
    Direction chooseNextDirection(Grid &grid);
};

#endif // RED_MONSTER_H