#include "RedMonster.h"
#include <algorithm>
#include <random>

RedMonster::RedMonster(Vector2 startPos)
    : Monster(startPos, false) // false = not a green dragon
{
    setSpeed(0.6f); // Slightly slower than base monster
}

void RedMonster::update()
{
    if (!active || isDead())
        return;

    updateMovement();
}

void RedMonster::draw()
{
    if (!active || isDead())
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    switch (getState())
    {
    case MonsterState::IN_TUNNEL:
        // Draw red monster
        DrawCircleV(center, size.x / 2 - 2, RED);
        // Draw monster eyes
        DrawCircleV({center.x - 6, center.y - 4}, 3, WHITE);
        DrawCircleV({center.x + 6, center.y - 4}, 3, WHITE);
        DrawCircleV({center.x - 6, center.y - 4}, 1, BLACK);
        DrawCircleV({center.x + 6, center.y - 4}, 1, BLACK);
        break;

    case MonsterState::DISEMBODIED:
        // Draw floating eyes
        DrawCircleV({center.x - 8, center.y}, 4, WHITE);
        DrawCircleV({center.x + 8, center.y}, 4, WHITE);
        DrawCircleV({center.x - 8, center.y}, 2, RED);
        DrawCircleV({center.x + 8, center.y}, 2, RED);
        break;

    case MonsterState::DEAD:
        // Don't draw anything for dead monsters
        break;
    }
}

void RedMonster::updateAI(Vector2 playerPos, Grid &grid)
{
    if (!active || isDead())
        return;

    // Use the base class AI with player position
    Monster::updateAI(grid, playerPos);
}

Direction RedMonster::chooseNextDirection(Grid &grid)
{
    // Simple AI: get valid directions and choose randomly
    std::vector<Direction> validDirs;
    std::vector<Direction> allDirs = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirs)
    {
        Vector2 testPos = position;
        float moveDistance = static_cast<float>(grid.getTileSize());

        switch (dir)
        {
        case Direction::UP:
            testPos.y -= moveDistance;
            break;
        case Direction::DOWN:
            testPos.y += moveDistance;
            break;
        case Direction::LEFT:
            testPos.x -= moveDistance;
            break;
        case Direction::RIGHT:
            testPos.x += moveDistance;
            break;
        default:
            continue;
        }

        if (canMoveTo(testPos, grid))
        {
            validDirs.push_back(dir);
        }
    }

    if (validDirs.empty())
        return Direction::NONE;

    // Random movement from valid directions
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(validDirs.size()) - 1);

    return validDirs[dis(gen)];
}