#include "Harpoon.h"
#include "Sprite.h"
#include <cmath>

Harpoon::Harpoon(Vector2 startPos, Direction dir)
    : GameObject(startPos, {8, 8}), // Small projectile size
      direction(dir),
      speed(4.0f),
      maxRange(200.0f), // Maximum range in pixels
      travelDistance(0.0f),
      fired(false)
{
    active = false; // Start inactive
}

void Harpoon::update()
{
    if (!active || !fired)
        return;

    updateMovement();

    // Check if harpoon has traveled too far
    if (travelDistance >= maxRange)
    {
        deactivate();
    }
}

void Harpoon::draw()
{
    if (!active || !fired)
        return;

    // Use the Sprite class to draw the harpoon
    Sprite::drawHarpoon(position, direction, size);
}

void Harpoon::fire(Vector2 startPos, Direction dir)
{
    position = startPos;
    direction = dir;
    active = true;
    fired = true;
    travelDistance = 0.0f;
}

Direction Harpoon::getDirection() const
{
    return direction;
}

bool Harpoon::isHarpoonActive() const
{
    return active && fired;
}

void Harpoon::deactivate()
{
    active = false;
    fired = false;
    travelDistance = 0.0f;
}

bool Harpoon::shouldDestroy(const Grid &grid) const
{
    if (!active || !fired)
        return false;

    // Check if out of bounds
    if (!isWithinBounds(grid))
        return true;

    // Check if hit a rock or earth (can only travel through tunnels)
    Vector2 gridPos = grid.worldToGrid(position);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (grid.isValidPosition(gridX, gridY))
    {
        TileType tileType = grid.getTile(gridX, gridY);
        // Harpoon should be destroyed if it hits rock or earth (not tunnel)
        if (tileType == TileType::ROCK || tileType == TileType::EARTH)
        {
            return true;
        }
    }

    return false;
}

void Harpoon::reset()
{
    deactivate();
    position = {0, 0};
    direction = Direction::NONE;
}

float Harpoon::getSpeed() const
{
    return speed;
}

void Harpoon::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Harpoon::updateMovement()
{
    Vector2 oldPosition = position;
    Vector2 newPosition = position;

    // Calculate the new position based on direction
    switch (direction)
    {
    case Direction::UP:
        newPosition.y -= speed;
        break;
    case Direction::DOWN:
        newPosition.y += speed;
        break;
    case Direction::LEFT:
        newPosition.x -= speed;
        break;
    case Direction::RIGHT:
        newPosition.x += speed;
        break;
    default:
        break;
    }

    // Update position (the shouldDestroy check in GamePlay will handle stopping the harpoon)
    position = newPosition;

    // Calculate travel distance
    float dx = position.x - oldPosition.x;
    float dy = position.y - oldPosition.y;
    travelDistance += std::sqrt(dx * dx + dy * dy);
}

bool Harpoon::isWithinBounds(const Grid &grid) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           position.y + size.y <= grid.getHeight() * grid.getTileSize();
}