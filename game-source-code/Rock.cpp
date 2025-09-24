#include "Rock.h"
#include "Sprite.h"
#include <cmath>

const float Rock::FALL_DELAY_TIME = 1.0f; // 1 second delay before falling
const float Rock::FALL_SPEED = 3.0f;      // Fall speed

Rock::Rock(Vector2 startPos)
    : GameObject(startPos, {32, 32}), // Same size as tile
      currentState(RockState::STATIONARY),
      speed(FALL_SPEED),
      fallDelay(0.0f),
      fallTimer(0.0f),
      originalPosition(startPos)
{
}

void Rock::update()
{
    if (!active)
        return;

    switch (currentState)
    {
    case RockState::STATIONARY:
        // Just sitting there, waiting
        break;

    case RockState::FALLING:
        // Update fall timer
        if (fallTimer < FALL_DELAY_TIME)
        {
            fallTimer += GetFrameTime();
        }
        else
        {
            // Start falling
            position.y += speed;
        }
        break;

    case RockState::FALLEN:
        // Rock has stopped falling
        break;
    }
}

void Rock::draw()
{
    if (!active)
        return;

    // Use the Sprite class to draw the rock
    Sprite::drawRock(position, size);

    // Add a visual indicator when rock is about to fall
    if (currentState == RockState::FALLING && fallTimer < FALL_DELAY_TIME)
    {
        // Draw warning indicator (flashing effect)
        float flashIntensity = (sinf(fallTimer * 10.0f) + 1.0f) / 2.0f; // 0 to 1
        Color warningColor = {255, static_cast<unsigned char>(255 * flashIntensity), 0, 128};

        // Draw warning outline
        Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};
        DrawCircleLinesV(center, size.x / 2 + 2, warningColor);
        DrawCircleLinesV(center, size.x / 2 + 4, warningColor);
    }

    // Debug: Draw a simple rectangle to verify rocks are being drawn
    // Remove this after confirming rocks are visible
    DrawRectangleLinesEx(Rectangle{position.x, position.y, size.x, size.y}, 2, RED);
}

void Rock::checkShouldFall(const Grid &grid)
{
    if (currentState != RockState::STATIONARY)
        return;

    // Check if the space directly below this rock is now a tunnel
    Vector2 gridPos = grid.worldToGrid(position);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    // Check the tile below
    if (grid.isValidPosition(gridX, gridY + 1))
    {
        TileType tileBelow = grid.getTile(gridX, gridY + 1);
        if (tileBelow == TileType::TUNNEL)
        {
            // Start falling!
            currentState = RockState::FALLING;
            fallTimer = 0.0f;
        }
    }
}

void Rock::updateFalling(const Grid &grid)
{
    if (currentState != RockState::FALLING)
        return;

    // Check if we should stop falling
    if (!hasGroundBelow(grid))
    {
        // Keep falling
        position.y += speed;
    }
    else
    {
        // Stop falling - snap to grid
        Vector2 gridPos = grid.worldToGrid(position);
        int gridY = static_cast<int>(gridPos.y);
        position.y = static_cast<float>(gridY * grid.getTileSize());

        currentState = RockState::FALLEN;
    }
}

bool Rock::hasGroundBelow(const Grid &grid) const
{
    Vector2 gridPos = grid.worldToGrid({position.x, position.y + size.y + 1});
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (!grid.isValidPosition(gridX, gridY))
        return true; // Bottom of screen

    TileType tileBelow = grid.getTile(gridX, gridY);
    return (tileBelow == TileType::EARTH || tileBelow == TileType::ROCK);
}

RockState Rock::getState() const
{
    return currentState;
}

Vector2 Rock::getGridPosition(const Grid &grid) const
{
    return grid.worldToGrid(position);
}

bool Rock::isFalling() const
{
    return currentState == RockState::FALLING && fallTimer >= FALL_DELAY_TIME;
}

void Rock::reset(Vector2 startPos)
{
    position = startPos;
    originalPosition = startPos;
    currentState = RockState::STATIONARY;
    fallTimer = 0.0f;
    active = true;
}

float Rock::getSpeed() const
{
    return speed;
}

void Rock::setSpeed(float newSpeed)
{
    speed = newSpeed;
}