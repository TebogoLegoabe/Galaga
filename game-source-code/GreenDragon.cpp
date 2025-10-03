#include "GreenDragon.h"
#include "Fire.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

const float GreenDragon::FIRE_BREATH_COOLDOWN_TIME = 2.0f; // 2 seconds between fire breaths
const float GreenDragon::FIRE_BREATH_RANGE = 128.0f;       // 4 tiles range

GreenDragon::GreenDragon(Vector2 startPos)
    : Monster(startPos, MonsterState::IN_TUNNEL),
      fireProjectile(std::make_unique<Fire>()),
      fireBreathCooldown(0.0f),
      fireBreathRange(FIRE_BREATH_RANGE)
{
    // Green dragons are slightly slower than red monsters but more dangerous
    setSpeed(1.3f);
}

void GreenDragon::update()
{
    // Call base class update first
    Monster::update();

    // Update fire breath cooldown
    updateFireBreathCooldown();

    // Update fire projectile
    if (fireProjectile)
    {
        fireProjectile->update();
    }
}

void GreenDragon::draw()
{
    if (!active || isDead())
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    switch (currentState)
    {
    case MonsterState::IN_TUNNEL:
    {
        // Draw as green dragon with scales and spikes
        DrawCircleV(center, size.x / 2 - 2, DARKGREEN);

        // Draw dragon scales/texture
        DrawCircleV({center.x - 4, center.y - 4}, 2, GREEN);
        DrawCircleV({center.x + 4, center.y - 4}, 2, GREEN);
        DrawCircleV({center.x, center.y + 4}, 2, GREEN);

        // Draw dragon spikes (more elaborate than red monster)
        Vector2 topSpike = {center.x, center.y - 12};
        DrawTriangle(
            {center.x - 6, center.y - 6},
            topSpike,
            {center.x + 6, center.y - 6},
            LIME);

        // Draw dragon eyes (yellow for dragons)
        DrawCircleV({center.x - 6, center.y - 2}, 3, YELLOW);
        DrawCircleV({center.x + 6, center.y - 2}, 3, YELLOW);
        DrawCircleV({center.x - 6, center.y - 2}, 1, BLACK);
        DrawCircleV({center.x + 6, center.y - 2}, 1, BLACK);

        // Draw nostrils/snout
        DrawCircleV({center.x - 2, center.y + 2}, 1, BLACK);
        DrawCircleV({center.x + 2, center.y + 2}, 1, BLACK);
        break;
    }

    case MonsterState::DISEMBODIED:
        // Draw as green floating eyes (different from red monsters)
        DrawCircleV({center.x - 8, center.y}, 6, LIME);
        DrawCircleV({center.x + 8, center.y}, 6, LIME);

        // Add ethereal glow effect
        DrawCircleV({center.x - 8, center.y}, 8, Fade(GREEN, 0.3f));
        DrawCircleV({center.x + 8, center.y}, 8, Fade(GREEN, 0.3f));

        DrawCircleV({center.x - 8, center.y}, 3, DARKGREEN); // Left pupil
        DrawCircleV({center.x + 8, center.y}, 3, DARKGREEN); // Right pupil
        break;

    case MonsterState::DEAD:
        // Don't draw dead dragons
        break;
    }

    // Draw fire projectile if active
    if (fireProjectile && fireProjectile->isFireActive())
    {
        fireProjectile->draw();
    }
}

void GreenDragon::updateAI(const Player &player, const Grid &grid, bool canBecomeDisembodied, std::function<void()> notifyDisembodied)
{
    if (currentState == MonsterState::DEAD)
        return;

    // Update AI every 0.2 seconds for more responsive behavior
    if (aiUpdateTimer < 0.2f)
        return;

    aiUpdateTimer = 0.0f;

    Vector2 playerPos = player.getPosition();
    float distanceToPlayer = calculateDistanceToPlayer(player);
    bool playerInSameTunnel = isPlayerInSameTunnel(player, grid);

    // Smart decision tree for green dragon behavior
    if (currentState == MonsterState::IN_TUNNEL)
    {
        // Priority 1: Try to breathe fire if we have a clear shot
        if (canBreatheFire() && isPlayerInFireRange(playerPos) && hasDirectTunnelPathToPlayer(player, grid))
        {
            if (breatheFire(playerPos))
            {
                return; // Successfully breathed fire, don't move this turn
            }
        }

        // Priority 2: Aggressive chase if player is close (within 3 tiles)
        if (distanceToPlayer < 96.0f) // 3 tiles
        {
            if (!isMoving)
            {
                Direction chaseDirection = findSmartDirectionToPlayer(player, grid);
                if (chaseDirection != Direction::NONE)
                {
                    move(chaseDirection, grid);
                    return;
                }
            }
        }
        // Priority 3: Medium distance tactical positioning (3-6 tiles)
        else if (distanceToPlayer <= 192.0f) // 6 tiles
        {
            if (!isMoving)
            {
                // 80% chance to move towards player, 20% tactical positioning
                if (rand() % 5 != 0) // 80% chance
                {
                    Direction chaseDirection = findSmartDirectionToPlayer(player, grid);
                    if (chaseDirection != Direction::NONE)
                    {
                        move(chaseDirection, grid);
                        return;
                    }
                }
                else
                {
                    // Tactical positioning - try to get into a good firing position
                    Direction tacticalDirection = findTacticalPosition(player, grid);
                    if (tacticalDirection != Direction::NONE)
                    {
                        move(tacticalDirection, grid);
                        return;
                    }
                }
            }
        }
        // Priority 4: Long distance - become disembodied or move strategically
        else if (distanceToPlayer > 192.0f) // More than 6 tiles
        {
            // Become disembodied if possible to close distance faster
            if (canBecomeDisembodied && stateTimer > 2.0f)
            {
                setState(MonsterState::DISEMBODIED);
                stateTimer = 0.0f;
                if (notifyDisembodied)
                {
                    notifyDisembodied();
                }
                return;
            }

            // Otherwise move towards player (60% chance)
            if (!isMoving && (rand() % 5 < 3)) // 60% chance
            {
                Direction chaseDirection = findSmartDirectionToPlayer(player, grid);
                if (chaseDirection != Direction::NONE)
                {
                    move(chaseDirection, grid);
                    return;
                }
            }
        }

        // Fallback: If we can't find a good direction, try any valid direction (rarely happens now)
        if (!isMoving && (rand() % 10 == 0)) // 10% chance
        {
            Direction fallbackDirection = findRandomValidDirection(grid);
            if (fallbackDirection != Direction::NONE)
            {
                move(fallbackDirection, grid);
            }
        }
    }
    else if (currentState == MonsterState::DISEMBODIED)
    {
        // Disembodied dragons are very aggressive - always move towards player
        if (stateTimer > 4.0f) // Stay disembodied longer for better positioning
        {
            Vector2 gridPos = grid.worldToGrid(position);
            if (grid.isTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y)))
            {
                setState(MonsterState::IN_TUNNEL);
                stateTimer = 0.0f;
            }
        }

        // Always pursue player aggressively when disembodied
        if (!isMoving)
        {
            Direction chaseDirection = findSmartDirectionToPlayer(player, grid);
            if (chaseDirection != Direction::NONE)
            {
                move(chaseDirection, grid);
            }
        }
    }

    // Update state timer
    stateTimer += GetFrameTime();
}

Fire &GreenDragon::getFire()
{
    return *fireProjectile;
}

const Fire &GreenDragon::getFire() const
{
    return *fireProjectile;
}

bool GreenDragon::canBreatheFire() const
{
    return fireBreathCooldown <= 0.0f && !fireProjectile->isFireActive();
}

bool GreenDragon::breatheFire(Vector2 playerPos)
{
    if (!canBreatheFire())
        return false;

    // Calculate fire direction towards player
    Direction fireDirection = calculateFireDirection(playerPos);
    if (fireDirection == Direction::NONE)
        return false;

    // Calculate fire starting position (from dragon's mouth)
    Vector2 fireStartPos = {
        position.x + size.x / 2 - 4, // Center horizontally
        position.y + size.y / 2 - 4  // Center vertically
    };

    // Adjust position based on direction
    switch (fireDirection)
    {
    case Direction::UP:
        fireStartPos.y = position.y - 8;
        break;
    case Direction::DOWN:
        fireStartPos.y = position.y + size.y;
        break;
    case Direction::LEFT:
        fireStartPos.x = position.x - 8;
        break;
    case Direction::RIGHT:
        fireStartPos.x = position.x + size.x;
        break;
    default:
        return false;
    }

    // Breathe fire
    fireProjectile->breathe(fireStartPos, fireDirection);
    fireBreathCooldown = FIRE_BREATH_COOLDOWN_TIME;

    return true;
}

Direction GreenDragon::findSmartDirectionToPlayer(const Player &player, const Grid &grid)
{
    Vector2 playerPos = player.getPosition();
    Vector2 monsterGridPos = grid.worldToGrid(position);
    Vector2 playerGridPos = grid.worldToGrid(playerPos);

    // Calculate direction vector to player
    float dx = playerGridPos.x - monsterGridPos.x;
    float dy = playerGridPos.y - monsterGridPos.y;

    // Prioritize directions that get us closer to player AND are valid moves
    std::vector<std::pair<Direction, float>> scoredDirections;

    Vector2 testPos;
    float moveDistance = static_cast<float>(grid.getTileSize());

    // Test all four directions and score them
    std::vector<Direction> allDirections = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        testPos = position;
        Vector2 directionVector = {0, 0};

        switch (dir)
        {
        case Direction::UP:
            testPos.y -= moveDistance;
            directionVector = {0, -1};
            break;
        case Direction::DOWN:
            testPos.y += moveDistance;
            directionVector = {0, 1};
            break;
        case Direction::LEFT:
            testPos.x -= moveDistance;
            directionVector = {-1, 0};
            break;
        case Direction::RIGHT:
            testPos.x += moveDistance;
            directionVector = {1, 0};
            break;
        default:
            continue;
        }

        if (!canMoveTo(testPos, grid))
            continue;

        // Score this direction based on multiple factors
        float score = 0;

        // Factor 1: Distance reduction (primary factor) - higher score for moves that get us closer
        Vector2 newGridPos = grid.worldToGrid(testPos);
        float newDistance = std::abs(playerGridPos.x - newGridPos.x) + std::abs(playerGridPos.y - newGridPos.y);
        float currentDistance = std::abs(playerGridPos.x - monsterGridPos.x) + std::abs(playerGridPos.y - monsterGridPos.y);
        float distanceImprovement = currentDistance - newDistance;
        score += distanceImprovement * 100; // High weight for distance improvement

        // Factor 2: Direction alignment - bonus for moving directly towards player
        float dotProduct = (directionVector.x * dx + directionVector.y * dy);
        if (dotProduct > 0) // Moving towards player
        {
            score += dotProduct * 50; // Bonus for alignment with player direction
        }

        // Factor 3: Fire line potential - bonus if this move gives us a clear fire path
        Vector2 futurePos = testPos;
        Vector2 futurePosCenter = {futurePos.x + size.x / 2, futurePos.y + size.y / 2};

        // Temporarily set position to test fire path
        Vector2 oldPos = position;
        position = futurePos;
        bool hasFirePath = hasDirectTunnelPathToPlayer(player, grid);
        position = oldPos; // Restore position

        if (hasFirePath && isPlayerInFireRange(playerPos))
        {
            score += 75; // Big bonus for getting into fire position
        }

        // Factor 4: Avoid getting stuck - penalty for moves that lead to dead ends
        int tunnelCount = 0;
        Vector2 surroundingPositions[4] = {
            {testPos.x, testPos.y - moveDistance}, // Up
            {testPos.x, testPos.y + moveDistance}, // Down
            {testPos.x - moveDistance, testPos.y}, // Left
            {testPos.x + moveDistance, testPos.y}  // Right
        };

        for (int i = 0; i < 4; i++)
        {
            Vector2 surroundingGrid = grid.worldToGrid(surroundingPositions[i]);
            int gx = static_cast<int>(surroundingGrid.x);
            int gy = static_cast<int>(surroundingGrid.y);
            if (grid.isValidPosition(gx, gy) && grid.isTunnel(gx, gy))
            {
                tunnelCount++;
            }
        }

        if (tunnelCount <= 1)
        {
            score -= 30; // Penalty for potential dead end
        }

        scoredDirections.push_back({dir, score});
    }

    // Sort by score (highest first)
    std::sort(scoredDirections.begin(), scoredDirections.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second > b.second;
              });

    // Return the highest-scored direction, or NONE if no valid directions
    return scoredDirections.empty() ? Direction::NONE : scoredDirections[0].first;
}

Direction GreenDragon::findTacticalPosition(const Player &player, const Grid &grid)
{
    Vector2 playerPos = player.getPosition();
    Vector2 playerGridPos = grid.worldToGrid(playerPos);

    // Find positions that would give us good fire lines to the player
    std::vector<std::pair<Direction, float>> tacticalMoves;

    float moveDistance = static_cast<float>(grid.getTileSize());
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

        if (!canMoveTo(testPos, grid))
            continue;

        float tacticalScore = 0;

        // Temporarily move to test position
        Vector2 oldPos = position;
        position = testPos;

        // Score based on fire line potential
        bool hasFirePath = hasDirectTunnelPathToPlayer(player, grid);
        bool inFireRange = isPlayerInFireRange(playerPos);

        position = oldPos; // Restore position

        if (hasFirePath && inFireRange)
        {
            tacticalScore += 100; // Perfect fire position
        }
        else if (hasFirePath)
        {
            tacticalScore += 60; // Good fire line but maybe too far
        }

        // Bonus for getting into same row/column as player (better fire lines)
        Vector2 testGridPos = grid.worldToGrid(testPos);
        if (static_cast<int>(testGridPos.x) == static_cast<int>(playerGridPos.x) ||
            static_cast<int>(testGridPos.y) == static_cast<int>(playerGridPos.y))
        {
            tacticalScore += 40;
        }

        // Small penalty for moving away from player (we want tactical position, not retreat)
        float currentDist = calculateDistanceToPlayer(player);
        Vector2 oldPosTemp = position;
        position = testPos;
        float newDist = calculateDistanceToPlayer(player);
        position = oldPosTemp;

        if (newDist > currentDist + 16) // Moving significantly away
        {
            tacticalScore -= 20;
        }

        if (tacticalScore > 0)
        {
            tacticalMoves.push_back({dir, tacticalScore});
        }
    }

    // Sort by tactical score
    std::sort(tacticalMoves.begin(), tacticalMoves.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second > b.second;
              });

    return tacticalMoves.empty() ? Direction::NONE : tacticalMoves[0].first;
}

Direction GreenDragon::findRandomValidDirection(const Grid &grid)
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

bool GreenDragon::isPlayerInFireRange(Vector2 playerPos) const
{
    float distance = std::sqrt(std::pow(playerPos.x - position.x, 2) +
                               std::pow(playerPos.y - position.y, 2));
    return distance <= fireBreathRange;
}

bool GreenDragon::hasDirectTunnelPathToPlayer(const Player &player, const Grid &grid) const
{
    Vector2 playerPos = player.getPosition();
    Vector2 dragonCenter = {position.x + size.x / 2, position.y + size.y / 2};
    Vector2 playerCenter = {playerPos.x + 16, playerPos.y + 16}; // Assuming player size ~32x32

    // Calculate direction to player
    float dx = playerCenter.x - dragonCenter.x;
    float dy = playerCenter.y - dragonCenter.y;

    // Determine if we should check horizontal or vertical path
    Direction fireDirection;
    bool checkHorizontal = std::abs(dx) > std::abs(dy);

    if (checkHorizontal)
    {
        fireDirection = (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        fireDirection = (dy > 0) ? Direction::DOWN : Direction::UP;
    }

    // Check if there's a clear tunnel path in the fire direction
    Vector2 currentPos = dragonCenter;
    Vector2 playerGridPos = grid.worldToGrid(playerPos);
    int tileSize = grid.getTileSize();

    // Step through tiles in the fire direction until we reach the player or hit an obstacle
    while (true)
    {
        // Move to next tile in fire direction
        switch (fireDirection)
        {
        case Direction::UP:
            currentPos.y -= tileSize;
            break;
        case Direction::DOWN:
            currentPos.y += tileSize;
            break;
        case Direction::LEFT:
            currentPos.x -= tileSize;
            break;
        case Direction::RIGHT:
            currentPos.x += tileSize;
            break;
        default:
            return false;
        }

        // Convert to grid coordinates
        Vector2 currentGridPos = grid.worldToGrid(currentPos);
        int gridX = static_cast<int>(currentGridPos.x);
        int gridY = static_cast<int>(currentGridPos.y);

        // Check if we've gone out of bounds
        if (!grid.isValidPosition(gridX, gridY))
        {
            return false;
        }

        // Check if we've reached the player's tile
        if (gridX == static_cast<int>(playerGridPos.x) && gridY == static_cast<int>(playerGridPos.y))
        {
            // We've reached the player - check if this tile is a tunnel
            return grid.isTunnel(gridX, gridY);
        }

        // Check if current tile blocks fire (not a tunnel)
        if (!grid.isTunnel(gridX, gridY))
        {
            return false; // Fire is blocked by earth or rock
        }

        // Check if we've moved too far from the player
        float distanceToPlayer = std::sqrt(std::pow(currentPos.x - playerCenter.x, 2) +
                                           std::pow(currentPos.y - playerCenter.y, 2));
        if (distanceToPlayer > fireBreathRange)
        {
            return false; // Too far away
        }

        // If we're checking horizontally but the path curves vertically (or vice versa), stop
        Vector2 dirToPlayer = {playerCenter.x - currentPos.x, playerCenter.y - currentPos.y};
        if (checkHorizontal && std::abs(dirToPlayer.y) > tileSize / 2)
        {
            // Path is curving vertically too much
            return false;
        }
        else if (!checkHorizontal && std::abs(dirToPlayer.x) > tileSize / 2)
        {
            // Path is curving horizontally too much
            return false;
        }
    }
}

Direction GreenDragon::calculateFireDirection(Vector2 playerPos) const
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};
    Vector2 playerCenter = {playerPos.x + 16, playerPos.y + 16}; // Assuming player size ~32x32

    float dx = playerCenter.x - center.x;
    float dy = playerCenter.y - center.y;

    // Determine primary direction based on largest component
    if (std::abs(dx) > std::abs(dy))
    {
        return (dx > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        return (dy > 0) ? Direction::DOWN : Direction::UP;
    }
}

void GreenDragon::updateFireBreathCooldown()
{
    if (fireBreathCooldown > 0.0f)
    {
        fireBreathCooldown -= GetFrameTime();
        if (fireBreathCooldown < 0.0f)
        {
            fireBreathCooldown = 0.0f;
        }
    }
}