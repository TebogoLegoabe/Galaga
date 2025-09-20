#include "Monster.h"
#include <cmath>
#include <algorithm>
#include <random>

Monster::Monster(Vector2 startPos, MonsterState state)
    : GameObject(startPos, {28, 28}), // Same size as player for consistency
      currentState(state),
      speed(1.5f), // Slightly slower than player
      targetPosition(startPos),
      isMoving(false),
      stateTimer(0.0f),
      aiUpdateTimer(0.0f),
      lastDirection(Direction::NONE)
{
}

void Monster::update()
{
    updateMovement();
    updateStateTimer();

    // Update AI timer
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
    {
        // Draw as red monster (can be replaced with sprite later)
        DrawCircleV(center, size.x / 2 - 2, RED);

        // Draw simple face
        DrawCircleV({center.x - 6, center.y - 4}, 2, BLACK); // Left eye
        DrawCircleV({center.x + 6, center.y - 4}, 2, BLACK); // Right eye

        // Define dark red color manually
        Color darkRed = {139, 0, 0, 255};
        DrawCircleV({center.x, center.y + 4}, 3, darkRed); // Mouth
        break;
    }

    case MonsterState::DISEMBODIED:
        // Draw as floating eyes
        DrawCircleV({center.x - 8, center.y}, 6, WHITE);
        DrawCircleV({center.x + 8, center.y}, 6, WHITE);
        DrawCircleV({center.x - 8, center.y}, 3, BLACK); // Left pupil
        DrawCircleV({center.x + 8, center.y}, 3, BLACK); // Right pupil
        break;

    case MonsterState::DEAD:
        // Don't draw dead monsters
        break;
    }
}

void Monster::updateAI(const Player &player, const Grid &grid)
{
    if (currentState == MonsterState::DEAD)
        return;

    // Update AI every 0.3 seconds for more strategic timing
    if (aiUpdateTimer < 0.3f)
        return;

    aiUpdateTimer = 0.0f;

    // Strategic AI decision making
    float distanceToPlayer = calculateDistanceToPlayer(player);
    bool playerInSameTunnel = isPlayerInSameTunnel(player, grid);

    // Decision tree for monster behavior
    if (currentState == MonsterState::IN_TUNNEL)
    {
        // Sometimes stay defensive and wait in tunnel
        if (stateTimer < 2.0f && !playerInSameTunnel)
        {
            // Stay put for a bit - let player come to us
            return;
        }

        // If player is very close (within 3 tiles), become aggressive
        if (distanceToPlayer < 96.0f) // 3 tiles * 32 pixels
        {
            // Aggressive mode - chase directly
            if (!isMoving)
            {
                Direction moveDirection = findBestDirectionToPlayer(player, grid);
                if (moveDirection != Direction::NONE)
                {
                    move(moveDirection, grid);
                }
            }
        }
        // If player is far and not in same tunnel, consider becoming disembodied
        else if (shouldBecomeDisembodied(player, grid))
        {
            setState(MonsterState::DISEMBODIED);
            stateTimer = 0.0f;
        }
        // Medium distance - move but not too aggressively
        else if (!isMoving && (rand() % 3 == 0)) // 33% chance to move
        {
            Direction moveDirection = findBestDirectionToPlayer(player, grid);
            if (moveDirection == Direction::NONE)
            {
                moveDirection = findRandomValidDirection(grid);
            }
            if (moveDirection != Direction::NONE)
            {
                move(moveDirection, grid);
            }
        }
    }
    else if (currentState == MonsterState::DISEMBODIED)
    {
        // Disembodied monsters are more aggressive and move more frequently
        if (stateTimer > 4.0f) // Stay disembodied longer
        {
            Vector2 gridPos = grid.worldToGrid(position);
            if (grid.isTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y)))
            {
                setState(MonsterState::IN_TUNNEL);
                stateTimer = 0.0f;
            }
        }

        // Move towards player aggressively when disembodied
        if (!isMoving)
        {
            Direction moveDirection = findBestDirectionToPlayer(player, grid);
            if (moveDirection != Direction::NONE)
            {
                move(moveDirection, grid);
            }
        }
    }
}

bool Monster::move(Direction direction, const Grid &grid)
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

    // Set target position and start moving
    targetPosition = newPos;
    isMoving = true;
    lastDirection = direction;

    return true;
}

MonsterState Monster::getState() const
{
    return currentState;
}

void Monster::setState(MonsterState newState)
{
    currentState = newState;
}

Vector2 Monster::getGridPosition(const Grid &grid) const
{
    return grid.worldToGrid(position);
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

    TileType tileType = grid.getTile(gridX, gridY);

    switch (currentState)
    {
    case MonsterState::IN_TUNNEL:
        // In tunnel monsters can only move through tunnels
        return tileType == TileType::TUNNEL;

    case MonsterState::DISEMBODIED:
        // Disembodied monsters can move through earth and tunnels, but not rocks
        return tileType != TileType::ROCK;

    case MonsterState::DEAD:
        return false;

    default:
        return false;
    }
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

float Monster::getSpeed() const
{
    return speed;
}

void Monster::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

bool Monster::isDead() const
{
    return currentState == MonsterState::DEAD;
}

Direction Monster::findBestDirectionToPlayer(const Player &player, const Grid &grid)
{
    Vector2 playerPos = player.getPosition();
    Vector2 monsterGridPos = grid.worldToGrid(position);
    Vector2 playerGridPos = grid.worldToGrid(playerPos);

    // Calculate direction to player
    float dx = playerGridPos.x - monsterGridPos.x;
    float dy = playerGridPos.y - monsterGridPos.y;

    // Prioritize directions based on distance to player
    std::vector<std::pair<Direction, float>> directions;

    // Calculate distance for each possible direction
    Vector2 testPos;
    float moveDistance = static_cast<float>(grid.getTileSize());

    // Up
    testPos = {position.x, position.y - moveDistance};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::UP, dist});
    }

    // Down
    testPos = {position.x, position.y + moveDistance};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::DOWN, dist});
    }

    // Left
    testPos = {position.x - moveDistance, position.y};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::LEFT, dist});
    }

    // Right
    testPos = {position.x + moveDistance, position.y};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::RIGHT, dist});
    }

    // Sort by distance (shortest first)
    std::sort(directions.begin(), directions.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second < b.second;
              });

    // Return the best direction, or NONE if no valid directions
    return directions.empty() ? Direction::NONE : directions[0].first;
}

Direction Monster::findRandomValidDirection(const Grid &grid)
{
    std::vector<Direction> validDirections;
    float moveDistance = static_cast<float>(grid.getTileSize());

    // Check all directions
    std::vector<Direction> allDirections = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        Vector2 testPos = position;
        switch (dir)
        {
        case Direction::UP:
            testPos.y -= moveDistance;
            break;
        case Direction::DOWN:
            testPos.y += moveDistance;
            break;
        case Direction::LEFT:
            testPos.x -= moveDistance;
            break;
        case Direction::RIGHT:
            testPos.x += moveDistance;
            break;
        default:
            continue;
        }

        if (canMoveTo(testPos, grid))
        {
            validDirections.push_back(dir);
        }
    }

    if (validDirections.empty())
        return Direction::NONE;

    // Return random valid direction
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validDirections.size() - 1);

    return validDirections[dis(gen)];
}

bool Monster::shouldBecomeDisembodied(const Player &player, const Grid &grid)
{
    // Only in-tunnel monsters can become disembodied
    if (currentState != MonsterState::IN_TUNNEL)
        return false;

    // Don't become disembodied too frequently
    if (stateTimer < 3.0f)
        return false;

    // Higher chance to become disembodied if player is far away
    float distance = calculateDistanceToPlayer(player);

    // If player is very close, stay in tunnel to be threatening
    if (distance < 64.0f) // 2 tiles
        return false;

    // If player is in different tunnel system and far away, become disembodied
    if (!isPlayerInSameTunnel(player, grid) && distance > 128.0f) // 4 tiles
    {
        return (rand() % 4 == 0); // 25% chance
    }

    return false;
}

void Monster::updateMovement()
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

void Monster::updateStateTimer()
{
    stateTimer += GetFrameTime();
}

bool Monster::isWithinGridBounds(Vector2 worldPos, const Grid &grid) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x + size.x <= grid.getWidth() * grid.getTileSize() &&
           worldPos.y + size.y <= grid.getHeight() * grid.getTileSize();
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
    // Simple check: are they in the same tunnel system?
    // This could be improved with more sophisticated pathfinding
    Vector2 playerGridPos = grid.worldToGrid(player.getPosition());
    Vector2 monsterGridPos = grid.worldToGrid(position);

    // Check if there's a path of tunnels between monster and player
    // For simplicity, just check if they're in the same row or column of tunnels
    return (static_cast<int>(playerGridPos.x) == static_cast<int>(monsterGridPos.x)) ||
           (static_cast<int>(playerGridPos.y) == static_cast<int>(monsterGridPos.y));
}