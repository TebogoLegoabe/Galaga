#include "GreenDragon.h"
#include "Fire.h"
#include "Player.h"
#include "PathFinding.h"
#include "TacticalAI.h"
#include <cmath>
#include <functional>

const float GreenDragon::FIRE_BREATH_COOLDOWN_TIME = 2.0f;
const float GreenDragon::FIRE_BREATH_RANGE = 128.0f;

GreenDragon::GreenDragon(Vector2 startPos)
    : Monster(startPos, MonsterState::IN_TUNNEL),
      fireProjectile(std::make_unique<Fire>()),
      fireBreathCooldown(0.0f),
      fireBreathRange(FIRE_BREATH_RANGE)
{
    setSpeed(1.3f);
}

void GreenDragon::update()
{
    Monster::update();
    updateFireBreathCooldown();

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
        DrawCircleV(center, size.x / 2 - 2, DARKGREEN);
        DrawCircleV({center.x - 4, center.y - 4}, 2, GREEN);
        DrawCircleV({center.x + 4, center.y - 4}, 2, GREEN);
        DrawCircleV({center.x, center.y + 4}, 2, GREEN);

        Vector2 topSpike = {center.x, center.y - 12};
        DrawTriangle({center.x - 6, center.y - 6}, topSpike, {center.x + 6, center.y - 6}, LIME);

        DrawCircleV({center.x - 6, center.y - 2}, 3, YELLOW);
        DrawCircleV({center.x + 6, center.y - 2}, 3, YELLOW);
        DrawCircleV({center.x - 6, center.y - 2}, 1, BLACK);
        DrawCircleV({center.x + 6, center.y - 2}, 1, BLACK);

        DrawCircleV({center.x - 2, center.y + 2}, 1, BLACK);
        DrawCircleV({center.x + 2, center.y + 2}, 1, BLACK);
        break;
    }

    case MonsterState::DISEMBODIED:
        DrawCircleV({center.x - 8, center.y}, 6, LIME);
        DrawCircleV({center.x + 8, center.y}, 6, LIME);
        DrawCircleV({center.x - 8, center.y}, 8, Fade(GREEN, 0.3f));
        DrawCircleV({center.x + 8, center.y}, 8, Fade(GREEN, 0.3f));
        DrawCircleV({center.x - 8, center.y}, 3, DARKGREEN);
        DrawCircleV({center.x + 8, center.y}, 3, DARKGREEN);
        break;

    case MonsterState::DEAD:
        break;
    }

    if (fireProjectile && fireProjectile->isFireActive())
    {
        fireProjectile->draw();
    }
}

void GreenDragon::updateAI(const Player &player, const Grid &grid, bool canBecomeDisembodied,
                           std::function<void()> notifyDisembodied)
{
    if (currentState == MonsterState::DEAD)
        return;

    if (aiUpdateTimer < 0.2f)
        return;

    aiUpdateTimer = 0.0f;

    if (currentState == MonsterState::IN_TUNNEL)
    {
        handleInTunnelAI(player, grid, canBecomeDisembodied, notifyDisembodied);
    }
    else if (currentState == MonsterState::DISEMBODIED)
    {
        handleDisembodiedAI(player, grid);
    }

    stateTimer += GetFrameTime();
}

void GreenDragon::handleInTunnelAI(const Player &player, const Grid &grid,
                                   bool canBecomeDisembodied, std::function<void()> notifyDisembodied)
{
    Vector2 playerPos = player.getPosition();
    float distanceToPlayer = calculateDistanceToPlayer(player);

    // Priority 1: Fire if possible
    if (canBreatheFire() && TacticalAI::isInRange(position, playerPos, fireBreathRange) &&
        hasDirectTunnelPathToPlayer(player, grid))
    {
        if (breatheFire(playerPos))
        {
            return;
        }
    }

    // Priority 2: Chase if close
    if (distanceToPlayer < 96.0f)
    {
        if (!isMoving)
        {
            auto canMoveFunc = [this, &grid](Vector2 pos)
            { return canMoveTo(pos, grid); };
            Direction chaseDirection = PathFinding::findBestDirectionToTarget(
                position, playerPos, grid, canMoveFunc);

            if (chaseDirection != Direction::NONE)
            {
                move(chaseDirection, grid);
                return;
            }
        }
    }
    // Priority 3: Tactical positioning
    else if (distanceToPlayer <= 192.0f)
    {
        if (!isMoving)
        {
            if (rand() % 5 != 0)
            {
                auto canMoveFunc = [this, &grid](Vector2 pos)
                { return canMoveTo(pos, grid); };
                Direction chaseDirection = PathFinding::findBestDirectionToTarget(
                    position, playerPos, grid, canMoveFunc);

                if (chaseDirection != Direction::NONE)
                {
                    move(chaseDirection, grid);
                    return;
                }
            }
            else
            {
                auto canMoveFunc = [this, &grid](Vector2 pos)
                { return canMoveTo(pos, grid); };
                auto hasFireLineFunc = [this, &player, &grid](Vector2 pos)
                {
                    Vector2 oldPos = position;
                    position = pos;
                    bool hasLine = hasDirectTunnelPathToPlayer(player, grid);
                    position = oldPos;
                    return hasLine;
                };

                Direction tacticalDirection = TacticalAI::findTacticalFirePosition(
                    position, playerPos, grid, canMoveFunc, hasFireLineFunc, fireBreathRange);

                if (tacticalDirection != Direction::NONE)
                {
                    move(tacticalDirection, grid);
                    return;
                }
            }
        }
    }
    // Priority 4: Become disembodied if far
    else if (distanceToPlayer > 192.0f)
    {
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

        if (!isMoving && (rand() % 5 < 3))
        {
            auto canMoveFunc = [this, &grid](Vector2 pos)
            { return canMoveTo(pos, grid); };
            Direction chaseDirection = PathFinding::findBestDirectionToTarget(
                position, playerPos, grid, canMoveFunc);

            if (chaseDirection != Direction::NONE)
            {
                move(chaseDirection, grid);
                return;
            }
        }
    }

    // Fallback
    if (!isMoving && (rand() % 10 == 0))
    {
        auto canMoveFunc = [this, &grid](Vector2 pos)
        { return canMoveTo(pos, grid); };
        Direction fallbackDirection = PathFinding::findRandomValidDirection(
            position, grid, canMoveFunc);

        if (fallbackDirection != Direction::NONE)
        {
            move(fallbackDirection, grid);
        }
    }
}

void GreenDragon::handleDisembodiedAI(const Player &player, const Grid &grid)
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
        auto canMoveFunc = [this, &grid](Vector2 pos)
        { return canMoveTo(pos, grid); };
        Direction chaseDirection = PathFinding::findBestDirectionToTarget(
            position, player.getPosition(), grid, canMoveFunc);

        if (chaseDirection != Direction::NONE)
        {
            move(chaseDirection, grid);
        }
    }
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

    Direction fireDirection = TacticalAI::calculateFacingDirection(position, playerPos);
    if (fireDirection == Direction::NONE)
        return false;

    Vector2 fireStartPos = {
        position.x + size.x / 2 - 4,
        position.y + size.y / 2 - 4};

    int offset = 8;
    switch (fireDirection)
    {
    case Direction::UP:
        fireStartPos.y = position.y - offset;
        break;
    case Direction::DOWN:
        fireStartPos.y = position.y + size.y;
        break;
    case Direction::LEFT:
        fireStartPos.x = position.x - offset;
        break;
    case Direction::RIGHT:
        fireStartPos.x = position.x + size.x;
        break;
    default:
        return false;
    }

    fireProjectile->breathe(fireStartPos, fireDirection);
    fireBreathCooldown = FIRE_BREATH_COOLDOWN_TIME;

    return true;
}

bool GreenDragon::hasDirectTunnelPathToPlayer(const Player &player, const Grid &grid) const
{
    Vector2 playerPos = player.getPosition();

    auto checkFunc = [&grid](int x, int y) -> bool
    {
        return grid.isTunnel(x, y);
    };

    return PathFinding::hasDirectPath(position, playerPos, grid, checkFunc);
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