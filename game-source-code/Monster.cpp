#include "Monster.h"
#include <cmath>

Monster::Monster(Vector2 startPos)
    : GameObject(startPos, {28, 28}),
      state(MonsterState::IN_TUNNEL),
      speed(0.5f),
      targetPosition(startPos),
      lastPosition(startPos),
      moveTimer(0.0f)
{
}

bool Monster::move(Direction direction, Grid &grid)
{
    if (direction == Direction::NONE)
        return false;

    // Calculate new position based on direction
    Vector2 newPos = position;
    float moveDistance = static_cast<float>(grid.getTileSize());

    switch (direction)
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
        return false;
    }

    // Check if the new position is valid
    if (!canMoveTo(newPos, grid))
        return false;

    // Store last position and move
    lastPosition = position;
    position = newPos;

    return true;
}

MonsterState Monster::getState() const
{
    return state;
}

void Monster::setState(MonsterState newState)
{
    state = newState;
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

void Monster::reset(Vector2 startPos)
{
    position = startPos;
    targetPosition = startPos;
    lastPosition = startPos;
    state = MonsterState::IN_TUNNEL;
    moveTimer = 0.0f;
    active = true;
}

float Monster::getSpeed() const
{
    return speed;
}

void Monster::setTargetPosition(Vector2 target)
{
    targetPosition = target;
}

bool Monster::isWithinGridBounds(Vector2 worldPos, const Grid &grid) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           worldPos.y + size.y <= grid.getHeight() * grid.getTileSize();
}

Direction Monster::getDirectionToTarget(Vector2 target) const
{
    float dx = target.x - position.x;
    float dy = target.y - position.y;

    // Choose direction with largest absolute difference
    if (std::abs(dx) > std::abs(dy))
    {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}