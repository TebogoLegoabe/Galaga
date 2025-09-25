#include "Rock.h"
#include "Sprite.h"
#include <cmath>

const float Rock::FALL_DELAY_TIME = 1.0f; // 1 second delay before falling
const float Rock::FALL_SPEED = 3.0f;      // Fall speed

Rock::Rock(Vector2 startPos)
    : GameObject(startPos, {32, 32}), // Same size as tile
      currentState(RockState::STATIONARY),
      speed(FALL_SPEED),
      fallTimer(0.0f),
      originalPosition(startPos),
      gridRef(nullptr)
{
}

void Rock::update()
{
    if (!active || gridRef == nullptr)
        return;

    switch (currentState)
    {
    case RockState::STATIONARY:
        if (!hasGroundBelow(*gridRef))
        {
            beginFalling();
        }
        break;

    case RockState::FALLING:
        if (fallTimer < FALL_DELAY_TIME)
        {
            fallTimer += GetFrameTime();
        }
        else
        {
            position.y += speed;

            if (hasGroundBelow(*gridRef))
            {
                Vector2 gridPos = gridRef->worldToGrid(position);
                int gridX = static_cast<int>(gridPos.x);
                int gridY = static_cast<int>(gridPos.y);

                position.y = static_cast<float>(gridY * gridRef->getTileSize());
                gridRef->setTile(gridX, gridY, TileType::ROCK);

                currentState = RockState::FALLEN;
                fallTimer = 0.0f;
            }
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

    if (currentState != RockState::FALLING)
        return;

    Sprite::drawRock(position, size);

    if (fallTimer < FALL_DELAY_TIME)
    {
        float flashIntensity = (sinf(fallTimer * 10.0f) + 1.0f) / 2.0f; // 0 to 1
        Color warningColor = {255, static_cast<unsigned char>(255 * flashIntensity), 0, 128};

        Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};
        DrawCircleLinesV(center, size.x / 2 + 2, warningColor);
        DrawCircleLinesV(center, size.x / 2 + 4, warningColor);
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

    if (gridRef != nullptr)
    {
        Vector2 gridPos = gridRef->worldToGrid(position);
        gridRef->setTile(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y), TileType::ROCK);
    }
}

float Rock::getSpeed() const
{
    return speed;
}

void Rock::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Rock::setGrid(Grid *gridPtr)
{
    gridRef = gridPtr;
}

void Rock::beginFalling()
{
    if (gridRef == nullptr)
        return;

    Vector2 gridPos = gridRef->worldToGrid(position);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    gridRef->setTile(gridX, gridY, TileType::TUNNEL);
    currentState = RockState::FALLING;
    fallTimer = 0.0f;
}
