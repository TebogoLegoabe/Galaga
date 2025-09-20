#include "Player.h"
#include <cmath>

Player::Player(Vector2 startPos)
    : GameObject(startPos, {28, 28}), // Slightly smaller than tile size for better fit
      facingDirection(Direction::RIGHT),
      speed(2.0f),
      targetPosition(startPos),
      isMoving(false),
      harpoon() // Default construct the harpoon
{
}

void Player::update()
{
    updateMovement();

    // Update harpoon
    harpoon.update();
}

void Player::draw()
{
    if (!active)
        return;

    // Use the Sprite class to draw Dig Dug
    Sprite::drawDigDug(position, facingDirection, size);

    // Draw harpoon
    harpoon.draw();
}

bool Player::move(Direction direction, Grid &grid)
{
    if (direction == Direction::NONE || isMoving)
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

    // Update facing direction
    facingDirection = direction;

    // Set target position and start moving
    targetPosition = newPos;
    isMoving = true;

    // Dig tunnel at the new position
    Vector2 gridPos = grid.worldToGrid(newPos);
    grid.digTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y));

    return true;
}

Direction Player::getFacingDirection() const
{
    return facingDirection;
}

bool Player::canMoveTo(Vector2 newPos, const Grid &grid) const
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

    // Player can move through earth (will dig) and tunnels, but not rocks
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

    // Reset harpoon
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

void Player::updateMovement()
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

bool Player::shootHarpoon()
{
    if (harpoon.isHarpoonActive())
        return false;

    // Calculate harpoon start position (in front of player)
    Vector2 harpoonStart = {position.x + size.x / 2 - 4, position.y + size.y / 2 - 4};

    switch (facingDirection)
    {
    case Direction::UP:
        harpoonStart.y -= 10;
        break;
    case Direction::DOWN:
        harpoonStart.y += 10;
        break;
    case Direction::LEFT:
        harpoonStart.x -= 10;
        break;
    case Direction::RIGHT:
        harpoonStart.x += 10;
        break;
    default:
        return false;
    }

    harpoon.fire(harpoonStart, facingDirection);
    return true;
}

Harpoon &Player::getHarpoon()
{
    return harpoon;
}

bool Player::canShoot() const
{
    return !harpoon.isHarpoonActive();
}