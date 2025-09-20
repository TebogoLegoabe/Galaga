#include "Monster.h"
#include <cmath>
#include <algorithm>
#include <random>

Monster::Monster(Vector2 startPos, bool isGreen)
    : GameObject(startPos, {28, 28}), // Same size as player for consistency
      state(MonsterState::IN_TUNNEL),
      facingDirection(Direction::LEFT),
      speed(1.5f), // Slower than player (player is 2.0f)
      targetPosition(startPos),
      isMoving(false),
      greenDragon(isGreen),
      moveTimer(0.0f),
      moveInterval(0.0f)
{
}

void Monster::update()
{
    if (state == MonsterState::DEAD || !active)
        return;

    updateMovement();
}

void Monster::draw()
{
    if (!active || state == MonsterState::DEAD)
        return;

    switch (state)
    {
    case MonsterState::IN_TUNNEL:
        if (greenDragon)
        {
            Sprite::drawGreenDragon(position, size);
        }
        else
        {
            Sprite::drawRedMonster(position, size);
        }
        break;

    case MonsterState::DISEMBODIED:
        Sprite::drawDisembodiedEyes(position, size, greenDragon);
        break;

    case MonsterState::DEAD:
        // Don't draw anything for dead monsters
        break;
    }
}

void Monster::updateAI(const Grid &grid, Vector2 playerPos)
{
    if (state == MonsterState::DEAD || !active)
        return;

    // Update move timer
    moveTimer += GetFrameTime();

    // Only try to move at intervals (makes monsters slower and more deliberate)
    if (moveTimer >= moveInterval && !isMoving)
    {
        Direction nextDirection = chooseNextDirection(grid, playerPos);

        if (nextDirection != Direction::NONE)
        {
            // Calculate new position
            Vector2 newPos = position;
            float moveDistance = static_cast<float>(grid.getTileSize());

            switch (nextDirection)
            {
            case Direction::UP:
                newPos.y -= moveDistance;
                break;
            case Direction::DOWN:
                newPos.y += moveDistance;
                break;
            case Direction::LEFT:
                newPos.x -= moveDistance;
                break;
            case Direction::RIGHT:
                newPos.x += moveDistance;
                break;
            default:
                break;
            }

            // Check if the new position is valid
            if (canMoveTo(newPos, grid))
            {
                facingDirection = nextDirection;
                targetPosition = newPos;
                isMoving = true;
                moveTimer = 0.0f; // Reset timer
            }
        }
        else
        {
            // If no valid move, try again sooner
            moveTimer = moveInterval - 0.3f;
        }
    }
}

void Monster::updateMovement()
{
    if (!isMoving)
        return;

    // Calculate distance to target
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= speed)
    {
        // Reached target
        position = targetPosition;
        isMoving = false;
    }
    else
    {
        // Move towards target
        position.x += (dx / distance) * speed;
        position.y += (dy / distance) * speed;
    }
}

Direction Monster::chooseNextDirection(const Grid &grid, Vector2 playerPos)
{
    // Get all valid directions (only in tunnels)
    std::vector<Direction> validDirections = getValidDirections(grid);

    if (validDirections.empty())
        return Direction::NONE;

    // Simple AI: move towards player if possible, otherwise random
    Direction towardsPlayer = getDirectionTowardsPlayer(playerPos);

    // Check if the direction towards player is valid
    if (std::find(validDirections.begin(), validDirections.end(), towardsPlayer) != validDirections.end())
    {
        // 70% chance to move towards player
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        if (dis(gen) <= 70)
        {
            return towardsPlayer;
        }
    }

    // Random movement from valid directions
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(validDirections.size()) - 1);

    return validDirections[dis(gen)];
}

std::vector<Direction> Monster::getValidDirections(const Grid &grid) const
{
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

    return validDirs;
}

Direction Monster::getDirectionTowardsPlayer(Vector2 playerPos) const
{
    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;

    // Choose the direction with the largest distance component
    if (std::abs(dx) > std::abs(dy))
    {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}

bool Monster::canMoveTo(Vector2 newPos, const Grid &grid) const
{
    // Check if within grid bounds
    if (!isWithinGridBounds(newPos, grid))
        return false;

    // Convert to grid coordinates
    Vector2 gridPos = grid.worldToGrid(newPos);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    // Check if the grid position is valid
    if (!grid.isValidPosition(gridX, gridY))
        return false;

    // Monsters can only move through tunnels (not earth or rocks)
    TileType tileType = grid.getTile(gridX, gridY);
    return tileType == TileType::TUNNEL;
}

Vector2 Monster::getGridPosition(const Grid &grid) const
{
    return grid.worldToGrid(position);
}

MonsterState Monster::getState() const
{
    return state;
}

void Monster::kill()
{
    state = MonsterState::DEAD;
    active = false;
}

bool Monster::isDead() const
{
    return state == MonsterState::DEAD;
}

float Monster::getSpeed() const
{
    return speed;
}

void Monster::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Monster::reset(Vector2 startPos)
{
    position = startPos;
    targetPosition = startPos;
    state = MonsterState::IN_TUNNEL;
    facingDirection = Direction::LEFT;
    isMoving = false;
    moveTimer = 0.0f;
    active = true;
}

bool Monster::isGreenDragon() const
{
    return greenDragon;
}

float Monster::getDistanceToPlayer(Vector2 playerPos) const
{
    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Monster::isWithinGridBounds(Vector2 worldPos, const Grid &grid) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           worldPos.y + size.y <= grid.getHeight() * grid.getTileSize();
}