#include "GameObject.h"
#include <cmath>

GameObject::GameObject(Vector2 pos, Vector2 objSize)
    : position(pos), size(objSize), active(true), speed(1.0f),
      targetPosition(pos), isMoving(false)
{
}

Rectangle GameObject::getBounds() const
{
    return Rectangle{position.x, position.y, size.x, size.y};
}

bool GameObject::isActive() const
{
    return active;
}

void GameObject::setActive(bool active)
{
    this->active = active;
}

Vector2 GameObject::getPosition() const
{
    return position;
}

void GameObject::setPosition(Vector2 newPos)
{
    position = newPos;
    targetPosition = newPos;
}

Vector2 GameObject::getSize() const
{
    return size;
}

// Movable interface implementation
bool GameObject::move(Direction direction, Grid &grid)
{
    if (direction == Direction::NONE || isMoving)
        return false;

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

    if (!canMoveTo(newPos, grid))
        return false;

    targetPosition = newPos;
    isMoving = true;
    return true;
}

bool GameObject::canMoveTo(Vector2 newPos, const Grid &grid) const
{
    if (!isWithinGridBounds(newPos, grid))
        return false;

    Vector2 gridPos = grid.worldToGrid(newPos);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    return grid.isValidPosition(gridX, gridY);
}

Vector2 GameObject::getGridPosition(const Grid &grid) const
{
    return grid.worldToGrid(position);
}

float GameObject::getSpeed() const
{
    return speed;
}

void GameObject::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void GameObject::updateMovement()
{
    if (!isMoving)
        return;

    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= speed)
    {
        position = targetPosition;
        isMoving = false;
    }
    else
    {
        position.x += (dx / distance) * speed;
        position.y += (dy / distance) * speed;
    }
}

bool GameObject::isWithinBounds(int screenWidth, int screenHeight) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x + size.x <= screenWidth &&
           position.y + size.y <= screenHeight;
}

bool GameObject::isWithinGridBounds(Vector2 worldPos, const Grid &grid) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           worldPos.y + size.y <= grid.getHeight() * grid.getTileSize();
}