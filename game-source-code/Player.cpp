#include "Player.h"
#include <cmath>

Player::Player(Vector2 startPos)
    : GameObject(startPos, {28, 28}),
      facingDirection(Direction::RIGHT),
      speed(2.0f),
      targetPosition(startPos),
      isMoving(false),
      harpoon(startPos, Direction::RIGHT)
{
}

void Player::update()
{
    updateMovement();
    harpoon.update();
}

void Player::draw()
{
    if (!active)
        return;

    Sprite::drawDigDug(position, size, facingDirection);
    harpoon.draw();
}

bool Player::move(Direction direction, Grid &grid)
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

    facingDirection = direction;
    targetPosition = newPos;
    isMoving = true;

    Vector2 gridPos = grid.worldToGrid(newPos);
    grid.digTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y));

    return true;
}

bool Player::shootHarpoon()
{
    if (!canShoot())
        return false;

    Vector2 harpoonStart = getHarpoonStartPosition();
    harpoon.fire(harpoonStart, facingDirection);

    return true;
}

Direction Player::getFacingDirection() const
{
    return facingDirection;
}

Harpoon &Player::getHarpoon()
{
    return harpoon;
}

const Harpoon &Player::getHarpoon() const
{
    return harpoon;
}

bool Player::canMoveTo(Vector2 newPos, const Grid &grid) const
{
    if (!isWithinGridBounds(newPos, grid))
        return false;

    Vector2 gridPos = grid.worldToGrid(newPos);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (!grid.isValidPosition(gridX, gridY))
        return false;

    TileType tileType = grid.getTile(gridX, gridY);
    return tileType != TileType::ROCK;
}

Vector2 Player::getGridPosition(const Grid &grid) const
{
    return grid.worldToGrid(position);
}

void Player::reset(Vector2 startPos)
{
    position = startPos;
    targetPosition = startPos;
    facingDirection = Direction::RIGHT;
    isMoving = false;
    active = true;
    harpoon.reset();
}

float Player::getSpeed() const
{
    return speed;
}

void Player::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

bool Player::canShoot() const
{
    return harpoon.getState() == HarpoonState::INACTIVE;
}

void Player::updateMovement()
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

void Player::digAtCurrentPosition(Grid &grid)
{
    Vector2 gridPos = grid.worldToGrid(position);
    grid.digTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y));
}

bool Player::isWithinGridBounds(Vector2 worldPos, const Grid &grid) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           worldPos.y + size.y <= grid.getHeight() * grid.getTileSize();
}

Vector2 Player::getHarpoonStartPosition() const
{
    Vector2 startPos = position;

    switch (facingDirection)
    {
    case Direction::UP:
        startPos.y -= 8;
        startPos.x += size.x / 2;
        break;
    case Direction::DOWN:
        startPos.y += size.y + 4;
        startPos.x += size.x / 2;
        break;
    case Direction::LEFT:
        startPos.x -= 8;
        startPos.y += size.y / 2;
        break;
    case Direction::RIGHT:
        startPos.x += size.x + 4;
        startPos.y += size.y / 2;
        break;
    default:
        break;
    }

    return startPos;
}