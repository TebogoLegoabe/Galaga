#include "RedMonster.h"
#include "Sprite.h"
#include <algorithm>
#include <random>

const float RedMonster::AI_UPDATE_INTERVAL = 0.2f; // Faster, more independent movement

RedMonster::RedMonster(Vector2 startPos)
    : Monster(startPos), aiUpdateTimer(0.0f)
{
    speed = 0.1f; // Slightly slower than player
}

void RedMonster::update()
{
    if (!active || state == MonsterState::DEAD)
        return;

    // Update AI timer
    aiUpdateTimer += GetFrameTime();
}

void RedMonster::draw()
{
    if (!active || state == MonsterState::DEAD)
        return;

    // Draw red monster sprite
    DrawCircle(position.x + size.x / 2, position.y + size.y / 2, size.x / 2 - 2, RED);

    // Draw eyes
    DrawCircle(position.x + size.x / 2 - 6, position.y + size.y / 2 - 4, 2, WHITE);
    DrawCircle(position.x + size.x / 2 + 6, position.y + size.y / 2 - 4, 2, WHITE);
    DrawPixel(position.x + size.x / 2 - 6, position.y + size.y / 2 - 4, BLACK);
    DrawPixel(position.x + size.x / 2 + 6, position.y + size.y / 2 - 4, BLACK);

    // Draw mouth
    DrawCircle(position.x + size.x / 2, position.y + size.y / 2 + 4, 3, DARKGRAY);
}

void RedMonster::updateAI(Vector2 playerPos, Grid &grid)
{
    if (!active || state == MonsterState::DEAD)
        return;

    // Only update AI at intervals
    if (aiUpdateTimer >= AI_UPDATE_INTERVAL)
    {
        aiUpdateTimer = 0.0f;

        // Set player as target but allow for more independent movement
        setTargetPosition(playerPos);

        // Choose direction and move independently
        Direction nextDir = chooseNextDirection(grid);
        if (nextDir != Direction::NONE)
        {
            move(nextDir, grid);
        }
    }
}

Direction RedMonster::chooseNextDirection(Grid &grid)
{
    // Get all valid directions
    std::vector<Direction> validDirs = getValidDirections(grid);

    if (validDirs.empty())
        return Direction::NONE;

    // If only one option, take it
    if (validDirs.size() == 1)
        return validDirs[0];

    // Prefer direction towards target
    Direction toTarget = getDirectionToTarget(targetPosition);

    // Check if target direction is valid
    auto it = std::find(validDirs.begin(), validDirs.end(), toTarget);
    if (it != validDirs.end())
    {
        return toTarget;
    }

    // If target direction not valid, pick a random valid direction
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validDirs.size() - 1);

    return validDirs[dis(gen)];
}

std::vector<Direction> RedMonster::getValidDirections(Grid &grid)
{
    std::vector<Direction> validDirs;
    std::vector<Direction> allDirs = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    float moveDistance = static_cast<float>(grid.getTileSize());

    for (Direction dir : allDirs)
    {
        Vector2 testPos = position;

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

    return validDirs;
}