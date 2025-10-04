#include "Monster.h"
#include "PathFinding.h"
#include <cmath>
#include <algorithm>

Monster::Monster(Vector2 startPos, MonsterState state)
    : GameObject(startPos, {28, 28}),
      currentState(state),
      stateTimer(0.0f),
      aiUpdateTimer(0.0f),
      lastDirection(Direction::NONE)
{
    speed = 1.5f; // Monster default speed
}

void Monster::update()
{
    updateMovement(); // Inherited from GameObject
    updateStateTimer();
    aiUpdateTimer += GetFrameTime();
}

void Monster::draw()
{
    if (!active)
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    switch (currentState)
    {
    case MonsterState::IN_TUNNEL:
        DrawCircleV(center, size.x / 2 - 2, RED);
        DrawCircleV({center.x - 6, center.y - 4}, 2, BLACK);
        DrawCircleV({center.x + 6, center.y - 4}, 2, BLACK);
        DrawCircleV({center.x, center.y + 4}, 3, Color{139, 0, 0, 255});
        break;

    case MonsterState::DISEMBODIED:
        DrawCircleV({center.x - 8, center.y}, 6, WHITE);
        DrawCircleV({center.x + 8, center.y}, 6, WHITE);
        DrawCircleV({center.x - 8, center.y}, 3, BLACK);
        DrawCircleV({center.x + 8, center.y}, 3, BLACK);
        break;

    case MonsterState::DEAD:
        break;
    }
}

void Monster::updateAI(const Player &player, Grid &grid, bool canBecomeDisembodied,
                       std::function<void()> notifyDisembodied)
{
    if (currentState == MonsterState::DEAD)
        return;

    if (aiUpdateTimer < 0.3f)
        return;

    aiUpdateTimer = 0.0f;

    float distanceToPlayer = calculateDistanceToPlayer(player);
    bool playerInSameTunnel = isPlayerInSameTunnel(player, grid);

    if (currentState == MonsterState::IN_TUNNEL)
    {
        if (stateTimer < 2.0f && !playerInSameTunnel)
            return;

        if (distanceToPlayer < 96.0f)
        {
            if (!isMoving)
            {
                Direction moveDirection = findBestDirectionToPlayer(player, grid);
                if (moveDirection != Direction::NONE)
                {
                    move(moveDirection, grid);
                }
            }
        }
        else if (canBecomeDisembodied && shouldBecomeDisembodied(player, grid))
        {
            setState(MonsterState::DISEMBODIED);
            stateTimer = 0.0f;
            if (notifyDisembodied)
                notifyDisembodied();
        }
        else if (!isMoving && (rand() % 3 == 0))
        {
            Direction moveDirection = findBestDirectionToPlayer(player, grid);
            if (moveDirection == Direction::NONE)
                moveDirection = findRandomValidDirection(grid);
            if (moveDirection != Direction::NONE)
                move(moveDirection, grid);
        }
    }
    else if (currentState == MonsterState::DISEMBODIED)
    {
        if (stateTimer > 4.0f)
        {
            Vector2 gridPos = grid.worldToGrid(position);
            if (grid.isTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y)))
            {
                setState(MonsterState::IN_TUNNEL);
                stateTimer = 0.0f;
            }
        }

        if (!isMoving)
        {
            Direction moveDirection = findBestDirectionToPlayer(player, grid);
            if (moveDirection != Direction::NONE)
                move(moveDirection, grid);
        }
    }
}

bool Monster::canMoveTo(Vector2 newPos, const Grid &grid) const
{
    if (!isWithinGridBounds(newPos, grid))
        return false;

    Vector2 gridPos = grid.worldToGrid(newPos);
    int gridX = static_cast<int>(gridPos.x);
    int gridY = static_cast<int>(gridPos.y);

    if (!grid.isValidPosition(gridX, gridY))
        return false;

    TileType tileType = grid.getTile(gridX, gridY);

    switch (currentState)
    {
    case MonsterState::IN_TUNNEL:
        return tileType == TileType::TUNNEL;
    case MonsterState::DISEMBODIED:
        return tileType != TileType::ROCK;
    case MonsterState::DEAD:
        return false;
    default:
        return false;
    }
}

MonsterState Monster::getState() const
{
    return currentState;
}

void Monster::setState(MonsterState newState)
{
    currentState = newState;
}

void Monster::reset(Vector2 startPos, MonsterState state)
{
    position = startPos;
    targetPosition = startPos;
    currentState = state;
    isMoving = false;
    stateTimer = 0.0f;
    aiUpdateTimer = 0.0f;
    lastDirection = Direction::NONE;
    active = true;
}

bool Monster::isDead() const
{
    return currentState == MonsterState::DEAD;
}

void Monster::updateStateTimer()
{
    stateTimer += GetFrameTime();
}

bool Monster::shouldBecomeDisembodied(const Player &player, const Grid &grid)
{
    if (currentState != MonsterState::IN_TUNNEL)
        return false;

    if (stateTimer < 3.0f)
        return false;

    float distance = calculateDistanceToPlayer(player);

    if (distance < 64.0f)
        return false;

    if (!isPlayerInSameTunnel(player, grid) && distance > 128.0f)
        return (rand() % 4 == 0);

    return false;
}

float Monster::calculateDistanceToPlayer(const Player &player) const
{
    Vector2 playerPos = player.getPosition();
    float dx = playerPos.x - position.x;
    float dy = playerPos.y - position.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Monster::isPlayerInSameTunnel(const Player &player, const Grid &grid) const
{
    Vector2 playerGridPos = grid.worldToGrid(player.getPosition());
    Vector2 monsterGridPos = grid.worldToGrid(position);

    return (static_cast<int>(playerGridPos.x) == static_cast<int>(monsterGridPos.x)) ||
           (static_cast<int>(playerGridPos.y) == static_cast<int>(monsterGridPos.y));
}

Direction Monster::findBestDirectionToPlayer(const Player &player, const Grid &grid)
{
    auto canMoveFunc = [this, &grid](Vector2 pos)
    { return canMoveTo(pos, grid); };

    return PathFinding::findBestDirectionToTarget(
        position, player.getPosition(), grid, canMoveFunc);
}

Direction Monster::findRandomValidDirection(const Grid &grid)
{
    auto canMoveFunc = [this, &grid](Vector2 pos)
    { return canMoveTo(pos, grid); };

    return PathFinding::findRandomValidDirection(position, grid, canMoveFunc);
}