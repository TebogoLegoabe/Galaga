#include "Harpoon.h"
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

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw harpoon based on direction
    switch (direction)
    {
    case Direction::UP:
        // Vertical harpoon pointing up
        DrawRectangle(position.x + 2, position.y, 4, size.y, YELLOW);
        DrawTriangle({center.x, position.y},
                     {position.x, position.y + 4},
                     {position.x + size.x, position.y + 4}, ORANGE);
        break;

    case Direction::DOWN:
        // Vertical harpoon pointing down
        DrawRectangle(position.x + 2, position.y, 4, size.y, YELLOW);
        DrawTriangle({center.x, position.y + size.y},
                     {position.x, position.y + size.y - 4},
                     {position.x + size.x, position.y + size.y - 4}, ORANGE);
        break;

    case Direction::LEFT:
        // Horizontal harpoon pointing left
        DrawRectangle(position.x, position.y + 2, size.x, 4, YELLOW);
        DrawTriangle({position.x, center.y},
                     {position.x + 4, position.y},
                     {position.x + 4, position.y + size.y}, ORANGE);
        break;

    case Direction::RIGHT:
        // Horizontal harpoon pointing right
        DrawRectangle(position.x, position.y + 2, size.x, 4, YELLOW);
        DrawTriangle({position.x + size.x, center.y},
                     {position.x + size.x - 4, position.y},
                     {position.x + size.x - 4, position.y + size.y}, ORANGE);
        break;

    default:
        // Default circle if direction is unknown
        DrawCircleV(center, size.x / 2, YELLOW);
        break;
    }
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

    // Check if hit a rock
    Vector2 gridPos = grid.worldToGrid(position);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (grid.isValidPosition(gridX, gridY))
    {
        TileType tileType = grid.getTile(gridX, gridY);
        if (tileType == TileType::ROCK)
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
        break;
    }

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