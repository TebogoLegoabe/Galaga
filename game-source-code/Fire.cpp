#include "Fire.h"
#include <cmath>

Fire::Fire(Vector2 startPos, Direction dir)
    : GameObject(startPos, {12, 12}), // Slightly larger than harpoon
      direction(dir),
      speed(3.0f),      // Slightly slower than harpoon
      maxRange(160.0f), // Maximum range in pixels (5 tiles)
      travelDistance(0.0f),
      burnTime(1.5f), // Fire lasts 1.5 seconds
      maxBurnTime(1.5f),
      breathing(false),
      animationTimer(0.0f)
{
    active = false; // Start inactive
}

void Fire::update()
{
    if (!active || !breathing)
        return;

    updateMovement();
    updateBurnTime();
    updateAnimation();

    // Check if fire should be destroyed
    if (burnTime <= 0.0f || travelDistance >= maxRange)
    {
        deactivate();
    }
}

void Fire::draw()
{
    if (!active || !breathing)
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Create animated fire effect with multiple colors
    float intensity = burnTime / maxBurnTime;               // Fade as time goes on
    float animOffset = sinf(animationTimer * 15.0f) * 3.0f; // Flickering animation

    // Draw fire layers for realistic effect
    Color fireCore = {255, static_cast<unsigned char>(200 * intensity), 0, static_cast<unsigned char>(255 * intensity)};
    Color fireOuter = {255, static_cast<unsigned char>(100 * intensity), 0, static_cast<unsigned char>(180 * intensity)};
    Color fireEdge = {255, 0, 0, static_cast<unsigned char>(120 * intensity)};

    switch (direction)
    {
    case Direction::UP:
    case Direction::DOWN:
    {
        // Vertical fire stream
        float height = size.y + animOffset;

        // Draw flame layers
        DrawRectangle(center.x - 4, position.y, 8, height, fireEdge);
        DrawRectangle(center.x - 3, position.y + 1, 6, height - 2, fireOuter);
        DrawRectangle(center.x - 2, position.y + 2, 4, height - 4, fireCore);

        // Add flickering particles
        for (int i = 0; i < 3; i++)
        {
            float particleOffset = sinf(animationTimer * 20.0f + i) * 2.0f;
            DrawCircleV({center.x + particleOffset, center.y + i * 3}, 1, fireCore);
        }
        break;
    }

    case Direction::LEFT:
    case Direction::RIGHT:
    {
        // Horizontal fire stream
        float width = size.x + animOffset;

        // Draw flame layers
        DrawRectangle(position.x, center.y - 4, width, 8, fireEdge);
        DrawRectangle(position.x + 1, center.y - 3, width - 2, 6, fireOuter);
        DrawRectangle(position.x + 2, center.y - 2, width - 4, 4, fireCore);

        // Add flickering particles
        for (int i = 0; i < 3; i++)
        {
            float particleOffset = sinf(animationTimer * 20.0f + i) * 2.0f;
            DrawCircleV({center.x + i * 3, center.y + particleOffset}, 1, fireCore);
        }
        break;
    }

    default:
        // Default circular fire
        DrawCircleV(center, size.x / 2 + animOffset, fireEdge);
        DrawCircleV(center, size.x / 3, fireOuter);
        DrawCircleV(center, size.x / 4, fireCore);
        break;
    }

    // Add glow effect around the fire
    DrawCircleV(center, size.x / 2 + 4, Fade(ORANGE, 0.2f * intensity));
}

void Fire::breathe(Vector2 startPos, Direction dir)
{
    position = startPos;
    direction = dir;
    active = true;
    breathing = true;
    travelDistance = 0.0f;
    burnTime = maxBurnTime;
    animationTimer = 0.0f;
}

Direction Fire::getDirection() const
{
    return direction;
}

bool Fire::isFireActive() const
{
    return active && breathing;
}

void Fire::deactivate()
{
    active = false;
    breathing = false;
    travelDistance = 0.0f;
    burnTime = maxBurnTime;
    animationTimer = 0.0f;
}

bool Fire::shouldDestroy(const Grid &grid) const
{
    if (!active || !breathing)
        return false;

    // Check if out of bounds
    if (!isWithinBounds(grid))
        return true;

    // Check if fire has entered a non-tunnel tile (earth or rock)
    Vector2 gridPos = grid.worldToGrid(position);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (grid.isValidPosition(gridX, gridY))
    {
        TileType tileType = grid.getTile(gridX, gridY);
        // Fire should be destroyed if it hits earth or rock (only travels through tunnels)
        if (tileType != TileType::TUNNEL)
        {
            return true;
        }
    }

    return false;
}

void Fire::reset()
{
    deactivate();
    position = {0, 0};
    direction = Direction::NONE;
}

float Fire::getSpeed() const
{
    return speed;
}

void Fire::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

float Fire::getBurnTime() const
{
    return burnTime;
}

void Fire::updateMovement()
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

void Fire::updateBurnTime()
{
    burnTime -= GetFrameTime();
    if (burnTime < 0.0f)
    {
        burnTime = 0.0f;
    }
}

bool Fire::isWithinBounds(const Grid &grid) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           position.y + size.y <= grid.getHeight() * grid.getTileSize();
}

void Fire::updateAnimation()
{
    animationTimer += GetFrameTime();
    // Keep animation timer in reasonable bounds
    if (animationTimer > 100.0f)
    {
        animationTimer = 0.0f;
    }
}