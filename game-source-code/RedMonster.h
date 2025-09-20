#ifndef RED_MONSTER_H
#define RED_MONSTER_H

#include "Monster.h"

/**
 * @brief Red monster implementation
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
     * @brief Update AI behavior to chase player
     * @param playerPos Player's current position
     * @param grid Reference to the game grid
     */
    void updateAI(Vector2 playerPos, Grid &grid);

private:
    float aiUpdateTimer;                   ///< Timer for AI decision updates
    static const float AI_UPDATE_INTERVAL; ///< How often to update AI (seconds)

    /**
     * @brief Choose next direction based on AI
     * @param grid Reference to the game grid
     * @return Direction to move
     */
    Direction chooseNextDirection(Grid &grid);

    /**
     * @brief Get all valid directions from current position
     * @param grid Reference to the game grid
     * @return Vector of valid directions
     */
    std::vector<Direction> getValidDirections(Grid &grid);
};

#endif // RED_MONSTER_H