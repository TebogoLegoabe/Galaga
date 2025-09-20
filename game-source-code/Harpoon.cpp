#include "Harpoon.h"

const float Harpoon::INFLATE_TIME = 2.0f; // 2 seconds to inflate

Harpoon::Harpoon(Vector2 startPos, Direction dir)
    : GameObject(startPos, {4, 4}), // Small harpoon size
      direction(dir),
      state(HarpoonState::INACTIVE),
      speed(4.0f),
      inflateTimer(0.0f)
{
}

void Harpoon::update()
{
    if (!active || state == HarpoonState::INACTIVE)
        return;

    switch (state)
    {
    case HarpoonState::FLYING:
        updateFlying();
        break;
    case HarpoonState::INFLATING:
        updateInflating();
        break;
    default:
        break;
    }
}

void Harpoon::draw()
{
    if (!active || state == HarpoonState::INACTIVE)
        return;

    switch (state)
    {
    case HarpoonState::FLYING:
        // Draw harpoon projectile
        DrawRectangle(position.x, position.y, size.x, size.y, YELLOW);
        // Draw harpoon line back to origin (simple line)
        if (direction == Direction::RIGHT)
            DrawLine(position.x, position.y + size.y / 2, position.x - 20, position.y + size.y / 2, YELLOW);
        else if (direction == Direction::LEFT)
            DrawLine(position.x + size.x, position.y + size.y / 2, position.x + size.x + 20, position.y + size.y / 2, YELLOW);
        else if (direction == Direction::UP)
            DrawLine(position.x + size.x / 2, position.y + size.y, position.x + size.x / 2, position.y + size.y + 20, YELLOW);
        else if (direction == Direction::DOWN)
            DrawLine(position.x + size.x / 2, position.y, position.x + size.x / 2, position.y - 20, YELLOW);
        break;
    case HarpoonState::INFLATING:
    {
        // Draw inflating effect (pulsing circle)
        float pulseSize = 10 + sin(inflateTimer * 5) * 5;
        DrawCircle(position.x + size.x / 2, position.y + size.y / 2, pulseSize, Color{255, 255, 0, 128});
        break;
    }
    default:
        break;
    }
}

void Harpoon::fire(Vector2 startPos, Direction fireDirection)
{
    position = startPos;
    direction = fireDirection;
    state = HarpoonState::FLYING;
    active = true;
    inflateTimer = 0.0f;
}

HarpoonState Harpoon::getState() const
{
    return state;
}

void Harpoon::setState(HarpoonState newState)
{
    state = newState;
    if (state == HarpoonState::INFLATING)
    {
        inflateTimer = 0.0f;
    }
}

Direction Harpoon::getDirection() const
{
    return direction;
}

bool Harpoon::isWithinBounds(const Grid &grid) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x < grid.getWidth() * grid.getTileSize() &&
           position.y < grid.getHeight() * grid.getTileSize();
}

void Harpoon::reset()
{
    state = HarpoonState::INACTIVE;
    active = false;
    inflateTimer = 0.0f;
}

void Harpoon::updateFlying()
{
    // Move harpoon in its direction
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

    // Add timeout for flying harpoons (auto-reset after 3 seconds)
    inflateTimer += GetFrameTime();
    if (inflateTimer >= 3.0f)
    {
        reset();
    }
}

void Harpoon::updateInflating()
{
    inflateTimer += GetFrameTime();

    if (inflateTimer >= INFLATE_TIME)
    {
        // Inflation complete - harpoon becomes inactive
        reset();
    }
}