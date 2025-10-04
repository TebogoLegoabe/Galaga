#ifndef TACTICAL_AI_H
#define TACTICAL_AI_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"
#include "Grid.h"
#include "Player.h"
#include <functional>

/**
 * @brief Handles tactical decision making for monsters
 */
class TacticalAI
{
public:
    /**
     * @brief Find tactical firing position for ranged attacks
     * @param currentPos Current world position
     * @param playerPos Player's world position
     * @param grid Reference to the game grid
     * @param canMoveFunc Function to check if movement is valid
     * @param hasFireLineFunc Function to check if position has clear fire line
     * @param fireRange Maximum firing range
     * @return Direction to move for tactical advantage
     */
    static Direction findTacticalFirePosition(
        Vector2 currentPos,
        Vector2 playerPos,
        const Grid &grid,
        std::function<bool(Vector2)> canMoveFunc,
        std::function<bool(Vector2)> hasFireLineFunc,
        float fireRange);

    /**
     * @brief Check if player is within firing range
     * @param currentPos Current world position
     * @param playerPos Player's world position
     * @param range Maximum range
     * @return true if player is in range
     */
    static bool isInRange(Vector2 currentPos, Vector2 playerPos, float range);

    /**
     * @brief Calculate direction to face target
     * @param currentPos Current world position
     * @param targetPos Target world position
     * @return Direction to face
     */
    static Direction calculateFacingDirection(Vector2 currentPos, Vector2 targetPos);

    /**
     * @brief Check if target is in same row or column (good for ranged attacks)
     * @param currentPos Current world position
     * @param targetPos Target world position
     * @param grid Reference to the game grid
     * @return true if aligned
     */
    static bool isAlignedWithTarget(Vector2 currentPos, Vector2 targetPos, const Grid &grid);

    /**
     * @brief Evaluate if a position is strategically good
     * @param pos Position to evaluate
     * @param playerPos Player's world position
     * @param grid Reference to the game grid
     * @param hasFireLineFunc Function to check fire line
     * @param fireRange Maximum firing range
     * @return Score for position (higher is better)
     */
    static float evaluatePositionScore(
        Vector2 pos,
        Vector2 playerPos,
        const Grid &grid,
        std::function<bool(Vector2)> hasFireLineFunc,
        float fireRange);
};

#endif // TACTICAL_AI_H