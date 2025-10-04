#include "TacticalAI.h"
#include "PathFinding.h"
#include <cmath>
#include <algorithm>

Direction TacticalAI::findTacticalFirePosition(
    Vector2 currentPos,
    Vector2 playerPos,
    const Grid &grid,
    std::function<bool(Vector2)> canMoveFunc,
    std::function<bool(Vector2)> hasFireLineFunc,
    float fireRange)
{
    std::vector<std::pair<Direction, float>> tacticalMoves;
    int tileSize = grid.getTileSize();

    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        Vector2 testPos = currentPos;

        switch (dir)
        {
        case Direction::UP:
            testPos.y -= tileSize;
            break;
        case Direction::DOWN:
            testPos.y += tileSize;
            break;
        case Direction::LEFT:
            testPos.x -= tileSize;
            break;
        case Direction::RIGHT:
            testPos.x += tileSize;
            break;
        default:
            continue;
        }

        if (!canMoveFunc(testPos))
            continue;

        float score = evaluatePositionScore(testPos, playerPos, grid, hasFireLineFunc, fireRange);

        if (score > 0)
        {
            tacticalMoves.push_back({dir, score});
        }
    }

    // Sort by tactical score
    std::sort(tacticalMoves.begin(), tacticalMoves.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second > b.second;
              });

    return tacticalMoves.empty() ? Direction::NONE : tacticalMoves[0].first;
}

bool TacticalAI::isInRange(Vector2 currentPos, Vector2 playerPos, float range)
{
    float distance = std::sqrt(
        std::pow(playerPos.x - currentPos.x, 2) +
        std::pow(playerPos.y - currentPos.y, 2));

    return distance <= range;
}

Direction TacticalAI::calculateFacingDirection(Vector2 currentPos, Vector2 targetPos)
{
    Vector2 center = {currentPos.x + 16, currentPos.y + 16};
    Vector2 targetCenter = {targetPos.x + 16, targetPos.y + 16};

    float dx = targetCenter.x - center.x;
    float dy = targetCenter.y - center.y;

    // Determine primary direction based on largest component
    if (std::abs(dx) > std::abs(dy))
    {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}

bool TacticalAI::isAlignedWithTarget(Vector2 currentPos, Vector2 targetPos, const Grid &grid)
{
    Vector2 currentGridPos = grid.worldToGrid(currentPos);
    Vector2 targetGridPos = grid.worldToGrid(targetPos);

    return (static_cast<int>(currentGridPos.x) == static_cast<int>(targetGridPos.x)) ||
           (static_cast<int>(currentGridPos.y) == static_cast<int>(targetGridPos.y));
}

float TacticalAI::evaluatePositionScore(
    Vector2 pos,
    Vector2 playerPos,
    const Grid &grid,
    std::function<bool(Vector2)> hasFireLineFunc,
    float fireRange)
{
    float score = 0;

    // Check if position has fire line to player
    bool hasFireLine = hasFireLineFunc(pos);
    bool inRange = isInRange(pos, playerPos, fireRange);

    if (hasFireLine && inRange)
    {
        score += 100; // Perfect fire position
    }
    else if (hasFireLine)
    {
        score += 60; // Good fire line but maybe too far
    }

    // Bonus for alignment with player (better fire lines)
    if (isAlignedWithTarget(pos, playerPos, grid))
    {
        score += 40;
    }

    // Calculate distance change
    Vector2 currentGridPos = grid.worldToGrid(pos);
    Vector2 playerGridPos = grid.worldToGrid(playerPos);

    float distanceToPlayer = PathFinding::manhattanDistance(currentGridPos, playerGridPos);

    // Prefer positions that are not too far from player
    if (distanceToPlayer > 6) // More than 6 tiles away
    {
        score -= 20;
    }

    return score;
}