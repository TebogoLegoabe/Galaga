#include "PathFinding.h"
#include <cmath>
#include <algorithm>
#include <random>

float PathFinding::manhattanDistance(Vector2 from, Vector2 to)
{
    return std::abs(to.x - from.x) + std::abs(to.y - from.y);
}

Direction PathFinding::findBestDirectionToTarget(
    Vector2 currentPos,
    Vector2 targetPos,
    const Grid &grid,
    std::function<bool(Vector2)> canMoveFunc)
{
    std::vector<std::pair<Direction, float>> scoredDirections;
    int tileSize = grid.getTileSize();

    // Test all four directions
    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        Vector2 testPos = getPositionAfterMove(currentPos, dir, tileSize);

        if (!canMoveFunc(testPos))
            continue;

        // Score this direction
        float score = scoreDirection(currentPos, testPos, targetPos, grid);
        scoredDirections.push_back({dir, score});
    }

    // Sort by score (highest first)
    std::sort(scoredDirections.begin(), scoredDirections.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second > b.second;
              });

    return scoredDirections.empty() ? Direction::NONE : scoredDirections[0].first;
}

std::vector<Direction> PathFinding::findValidDirections(
    Vector2 currentPos,
    const Grid &grid,
    std::function<bool(Vector2)> canMoveFunc)
{
    std::vector<Direction> validDirections;
    int tileSize = grid.getTileSize();

    std::vector<Direction> allDirections = {
        Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        Vector2 testPos = getPositionAfterMove(currentPos, dir, tileSize);
        if (canMoveFunc(testPos))
        {
            validDirections.push_back(dir);
        }
    }

    return validDirections;
}

Direction PathFinding::findRandomValidDirection(
    Vector2 currentPos,
    const Grid &grid,
    std::function<bool(Vector2)> canMoveFunc)
{
    std::vector<Direction> validDirections = findValidDirections(currentPos, grid, canMoveFunc);

    if (validDirections.empty())
        return Direction::NONE;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validDirections.size() - 1);

    return validDirections[dis(gen)];
}

bool PathFinding::hasDirectPath(
    Vector2 from,
    Vector2 to,
    const Grid &grid,
    std::function<bool(int, int)> checkFunc)
{
    Vector2 fromCenter = {from.x + 16, from.y + 16};
    Vector2 toCenter = {to.x + 16, to.y + 16};

    float dx = toCenter.x - fromCenter.x;
    float dy = toCenter.y - fromCenter.y;

    // Determine primary direction
    bool checkHorizontal = std::abs(dx) > std::abs(dy);
    int tileSize = grid.getTileSize();

    Vector2 currentPos = fromCenter;
    Vector2 targetGridPos = grid.worldToGrid(to);

    // Step through tiles
    int maxSteps = 20; // Prevent infinite loops
    int steps = 0;

    while (steps < maxSteps)
    {
        // Move in primary direction
        if (checkHorizontal)
        {
            currentPos.x += (dx > 0 ? tileSize : -tileSize);
        }
        else
        {
            currentPos.y += (dy > 0 ? tileSize : -tileSize);
        }

        Vector2 currentGridPos = grid.worldToGrid(currentPos);
        int gridX = static_cast<int>(currentGridPos.x);
        int gridY = static_cast<int>(currentGridPos.y);

        // Check bounds
        if (!grid.isValidPosition(gridX, gridY))
            return false;

        // Check if reached target
        if (gridX == static_cast<int>(targetGridPos.x) &&
            gridY == static_cast<int>(targetGridPos.y))
        {
            return checkFunc(gridX, gridY);
        }

        // Check if path is blocked
        if (!checkFunc(gridX, gridY))
            return false;

        // Check if path curves too much
        Vector2 dirToTarget = {toCenter.x - currentPos.x, toCenter.y - currentPos.y};
        if (checkHorizontal && std::abs(dirToTarget.y) > tileSize / 2)
            return false;
        if (!checkHorizontal && std::abs(dirToTarget.x) > tileSize / 2)
            return false;

        steps++;
    }

    return false;
}

float PathFinding::scoreDirection(
    Vector2 currentPos,
    Vector2 testPos,
    Vector2 targetPos,
    const Grid &grid)
{
    float score = 0;

    // Factor 1: Distance reduction (primary)
    Vector2 currentGridPos = grid.worldToGrid(currentPos);
    Vector2 testGridPos = grid.worldToGrid(testPos);
    Vector2 targetGridPos = grid.worldToGrid(targetPos);

    float currentDistance = manhattanDistance(currentGridPos, targetGridPos);
    float newDistance = manhattanDistance(testGridPos, targetGridPos);
    float distanceImprovement = currentDistance - newDistance;

    score += distanceImprovement * 100;

    // Factor 2: Direction alignment
    float dx = targetGridPos.x - currentGridPos.x;
    float dy = targetGridPos.y - currentGridPos.y;

    Vector2 moveVector = {testGridPos.x - currentGridPos.x, testGridPos.y - currentGridPos.y};
    float dotProduct = (moveVector.x * dx + moveVector.y * dy);

    if (dotProduct > 0)
        score += dotProduct * 50;

    // Factor 3: Avoid dead ends
    if (isDeadEnd(testPos, grid))
        score -= 30;

    return score;
}

bool PathFinding::isDeadEnd(Vector2 pos, const Grid &grid)
{
    int tunnelCount = countTunnelNeighbors(pos, grid);
    return tunnelCount <= 1;
}

int PathFinding::countTunnelNeighbors(Vector2 pos, const Grid &grid)
{
    int tileSize = grid.getTileSize();
    int count = 0;

    Vector2 surroundingPositions[4] = {
        {pos.x, pos.y - tileSize}, // Up
        {pos.x, pos.y + tileSize}, // Down
        {pos.x - tileSize, pos.y}, // Left
        {pos.x + tileSize, pos.y}  // Right
    };

    for (const Vector2 &surroundingPos : surroundingPositions)
    {
        Vector2 gridPos = grid.worldToGrid(surroundingPos);
        int gx = static_cast<int>(gridPos.x);
        int gy = static_cast<int>(gridPos.y);

        if (grid.isValidPosition(gx, gy) && grid.isTunnel(gx, gy))
        {
            count++;
        }
    }

    return count;
}

Vector2 PathFinding::getPositionAfterMove(Vector2 pos, Direction dir, int tileSize)
{
    Vector2 newPos = pos;

    switch (dir)
    {
    case Direction::UP:
        newPos.y -= tileSize;
        break;
    case Direction::DOWN:
        newPos.y += tileSize;
        break;
    case Direction::LEFT:
        newPos.x -= tileSize;
        break;
    case Direction::RIGHT:
        newPos.x += tileSize;
        break;
    default:
        break;
    }

    return newPos;
}