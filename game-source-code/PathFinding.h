#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <raylib-cpp.hpp>
#include <vector>
#include <utility>
#include "GameEnums.h"
#include "Grid.h"
#include <functional>

/**
 * @brief Static utility class for pathfinding operations
 */
class PathFinding
{
public:
    /**
     * @brief Calculate Manhattan distance between two grid positions
     * @param from Starting grid position
     * @param to Target grid position
     * @return Manhattan distance
     */
    static float manhattanDistance(Vector2 from, Vector2 to);

    /**
     * @brief Find the best direction to move toward a target
     * @param currentPos Current world position
     * @param targetPos Target world position
     * @param grid Reference to the game grid
     * @param canMoveFunc Function to check if movement to a position is valid
     * @return Best direction to move
     */
    static Direction findBestDirectionToTarget(
        Vector2 currentPos,
        Vector2 targetPos,
        const Grid &grid,
        std::function<bool(Vector2)> canMoveFunc);

    /**
     * @brief Find all valid directions from current position
     * @param currentPos Current world position
     * @param grid Reference to the game grid
     * @param canMoveFunc Function to check if movement to a position is valid
     * @return Vector of valid directions
     */
    static std::vector<Direction> findValidDirections(
        Vector2 currentPos,
        const Grid &grid,
        std::function<bool(Vector2)> canMoveFunc);

    /**
     * @brief Find a random valid direction
     * @param currentPos Current world position
     * @param grid Reference to the game grid
     * @param canMoveFunc Function to check if movement to a position is valid
     * @return Random valid direction or NONE
     */
    static Direction findRandomValidDirection(
        Vector2 currentPos,
        const Grid &grid,
        std::function<bool(Vector2)> canMoveFunc);

    /**
     * @brief Check if there's a direct path between two positions
     * @param from Starting world position
     * @param to Target world position
     * @param grid Reference to the game grid
     * @param checkFunc Function to check if a tile blocks the path
     * @return true if there's a direct path
     */
    static bool hasDirectPath(
        Vector2 from,
        Vector2 to,
        const Grid &grid,
        std::function<bool(int, int)> checkFunc);

    /**
     * @brief Score a direction based on multiple factors
     * @param currentPos Current world position
     * @param testPos Test position after moving in a direction
     * @param targetPos Target world position
     * @param grid Reference to the game grid
     * @return Score for this direction (higher is better)
     */
    static float scoreDirection(
        Vector2 currentPos,
        Vector2 testPos,
        Vector2 targetPos,
        const Grid &grid);

    /**
     * @brief Check if a position would be a dead end
     * @param pos World position to check
     * @param grid Reference to the game grid
     * @return true if position has 1 or fewer exits
     */
    static bool isDeadEnd(Vector2 pos, const Grid &grid);

    /**
     * @brief Count number of tunnel neighbors around a position
     * @param pos World position to check
     * @param grid Reference to the game grid
     * @return Number of adjacent tunnels
     */
    static int countTunnelNeighbors(Vector2 pos, const Grid &grid);

private:
    /**
     * @brief Get world position after moving in a direction
     * @param pos Current world position
     * @param dir Direction to move
     * @param tileSize Size of one tile
     * @return New world position
     */
    static Vector2 getPositionAfterMove(Vector2 pos, Direction dir, int tileSize);
};

#endif // PATHFINDING_H