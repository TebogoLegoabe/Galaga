#include "Player.h"
#include <cmath>

const float Player::SHOOT_COOLDOWN_TIME = 0.5f; // Half second cooldown
const int Player::MAX_LIVES = 3;

Player::Player(Vector2 startPos)
    : GameObject(startPos, {28, 28}), // Slightly smaller than tile size for better fit
      facingDirection(Direction::RIGHT),
      speed(2.0f),
      targetPosition(startPos),
      isMoving(false),
      harpoon(std::make_unique<Harpoon>()),
      shootCooldown(0.0f)
{
}

void Player::update()
{
    updateMovement();
    updateShooting();

    // Update harpoon
    if (harpoon)
    {
        harpoon->update();

        // Check if harpoon should be deactivated
        // This will be handled by collision detection in GamePlay
    }
}

void Player::draw()
{
    if (!active)
        return;

    // Draw the player as a circle for now (can be replaced with sprite later)
    Color playerColor = BLUE;

    // Draw main body
    DrawCircleV(Vector2{position.x + size.x / 2, position.y + size.y / 2},
                size.x / 2 - 2, playerColor);

    // Draw a simple direction indicator
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};
    Vector2 directionIndicator = center;

    switch (facingDirection)
    {
    case Direction::UP:
        directionIndicator.y -= size.y / 3;
        break;
    case Direction::DOWN:
        directionIndicator.y += size.y / 3;
        break;
    case Direction::LEFT:
        directionIndicator.x -= size.x / 3;
        break;
    case Direction::RIGHT:
        directionIndicator.x += size.x / 3;
        break;
    default:
        break;
    }

    DrawCircleV(directionIndicator, 4, WHITE);

    // Draw harpoon if active
    if (harpoon)
    {
        harpoon->draw();
    }
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

bool Player::shoot()
{
    if (!canShoot())
        return false;

    // Calculate harpoon starting position (center of player)
    Vector2 harpoonPos = {
        position.x + size.x / 2 - 4, // Center horizontally, offset by half harpoon width
        position.y + size.y / 2 - 4  // Center vertically, offset by half harpoon height
    };

    // Adjust position slightly based on direction to make it look like it's coming from player
    switch (facingDirection)
    {
    case Direction::UP:
        harpoonPos.y = position.y - 8; // Start just above player
        break;
    case Direction::DOWN:
        harpoonPos.y = position.y + size.y; // Start just below player
        break;
    case Direction::LEFT:
        harpoonPos.x = position.x - 8; // Start just left of player
        break;
    case Direction::RIGHT:
        harpoonPos.x = position.x + size.x; // Start just right of player
        break;
    default:
        return false;
    }

    // Fire the harpoon
    harpoon->fire(harpoonPos, facingDirection);
    shootCooldown = SHOOT_COOLDOWN_TIME;

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
    shootCooldown = 0.0f;

    if (harpoon)
    {
        harpoon->reset();
    }
}

float Player::getSpeed() const
{
    return speed;
}

void Player::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

Harpoon &Player::getHarpoon()
{
    return *harpoon;
}

const Harpoon &Player::getHarpoon() const
{
    return *harpoon;
}

bool Player::canShoot() const
{
    return shootCooldown <= 0.0f && (!harpoon->isHarpoonActive());
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

void Player::updateShooting()
{
    // Update shoot cooldown
    if (shootCooldown > 0.0f)
    {
        shootCooldown -= GetFrameTime();
        if (shootCooldown < 0.0f)
        {
            shootCooldown = 0.0f;
        }
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

int Player::getLives() const
{
    return lives;
}

bool Player::loseLife()
{
    if (lives > 0)
    {
        lives--;
    }
    return lives > 0;
}

void Player::resetLives()
{
    lives = MAX_LIVES;
}

bool Player::isAlive() const
{
    return lives > 0;
}