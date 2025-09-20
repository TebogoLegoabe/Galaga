#include "Harpoon.h"
#include <cmath>

Harpoon::Harpoon(Vector2 startPos, Direction dir)
    : GameObject(startPos, {8, 8}), // Small harpoon size
      direction(dir),
      speed(4.0f), // Faster than player movement
      startPosition(startPos),
      maxRange(128.0f), // 4 tiles range
      isActive(false)
{
    active = false; // Start inactive
}

void Harpoon::update()
{
    if (!isActive || !active)
        return;

    // Move in the current direction
    switch (direction)
    {
    case Direction::UP:
        position.y -= speed;
        break;
    case Direction::DOWN:
        position.y += speed;
        break;
    case Direction::LEFT:
        position.x -= speed;
        break;
    case Direction::RIGHT:
        position.x += speed;
        break;
    default:
        stop();
        return;
    }

    // Check if we've reached maximum range
    float dx = position.x - startPosition.x;
    float dy = position.y - startPosition.y;
    float distanceTraveled = std::sqrt(dx * dx + dy * dy);

    if (distanceTraveled >= maxRange)
    {
        stop();
    }
}

void Harpoon::draw()
{
    if (!active || !isActive)
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw harpoon based on direction
    switch (direction)
    {
    case Direction::UP:
    case Direction::DOWN:
        // Vertical harpoon
        DrawRectangle(static_cast<int>(center.x - 1), static_cast<int>(position.y),
                      2, static_cast<int>(size.y), YELLOW);
        DrawCircle(static_cast<int>(center.x),
                   static_cast<int>(direction == Direction::UP ? position.y : position.y + size.y),
                   3, ORANGE);
        break;
    case Direction::LEFT:
    case Direction::RIGHT:
        // Horizontal harpoon
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(center.y - 1),
                      static_cast<int>(size.x), 2, YELLOW);
        DrawCircle(static_cast<int>(direction == Direction::LEFT ? position.x : position.x + size.x),
                   static_cast<int>(center.y), 3, ORANGE);
        break;
    default:
        break;
    }
}

void Harpoon::fire(Vector2 startPos, Direction dir)
{
    if (isActive) // Can't fire if already active
        return;

    position = startPos;
    startPosition = startPos;
    direction = dir;
    isActive = true;
    active = true;
}

bool Harpoon::isHarpoonActive() const
{
    return isActive && active;
}

void Harpoon::stop()
{
    isActive = false;
    active = false;
}

Direction Harpoon::getDirection() const
{
    return direction;
}

float Harpoon::getSpeed() const
{
    return speed;
}

bool Harpoon::hasReachedMaxRange(const Grid &grid) const
{
    if (!isActive)
        return false;

    // Check distance traveled
    float dx = position.x - startPosition.x;
    float dy = position.y - startPosition.y;
    float distanceTraveled = std::sqrt(dx * dx + dy * dy);

    if (distanceTraveled >= maxRange)
        return true;

    // Check if hitting a wall (earth or rock)
    return !canContinue(grid);
}

void Harpoon::reset()
{
    isActive = false;
    active = false;
    position = startPosition;
}

bool Harpoon::canContinue(const Grid &grid) const
{
    // Calculate next position
    Vector2 nextPos = position;

    switch (direction)
    {
    case Direction::UP:
        nextPos.y -= speed;
        break;
    case Direction::DOWN:
        nextPos.y += speed;
        break;
    case Direction::LEFT:
        nextPos.x -= speed;
        break;
    case Direction::RIGHT:
        nextPos.x += speed;
        break;
    default:
        return false;
    }

    // Check if next position is within bounds
    if (nextPos.x < 0 || nextPos.y < 0 ||
        nextPos.x >= grid.getWidth() * grid.getTileSize() ||
        nextPos.y >= grid.getHeight() * grid.getTileSize())
    {
        return false;
    }

    // Convert to grid coordinates
    Vector2 gridPos = grid.worldToGrid(nextPos);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (!grid.isValidPosition(gridX, gridY))
        return false;

    // Harpoon can travel through tunnels and earth, but not rocks
    TileType tileType = grid.getTile(gridX, gridY);
    return tileType != TileType::ROCK;
}

bool Harpoon::getIsActive() const
{
    return isActive;
}