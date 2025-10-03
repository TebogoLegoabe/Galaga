#ifndef GREEN_DRAGON_H
#define GREEN_DRAGON_H

#include "Monster.h"
#include <memory>
#include <random>

// Forward declaration to avoid circular dependency
class Fire;

/**
 * @brief Green dragon subclass with fire-breathing capability
 */
class GreenDragon : public Monster
{
public:
    /**
     * @brief Constructor for GreenDragon
     * @param startPos Starting position in world coordinates
     */
    GreenDragon(Vector2 startPos = {0, 0});

    /**
     * @brief Update the green dragon
     */
    void update() override;

    /**
     * @brief Draw the green dragon
     */
    void draw() override;

    /**
     * @brief Update monster AI to chase the player (GreenDragon specific)
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @param canBecomeDisembodied Whether the monster is allowed to become disembodied
     * @param notifyDisembodied Callback function to notify when monster becomes disembodied
     */
    void updateAI(const Player &player, const Grid &grid, bool canBecomeDisembodied, std::function<void()> notifyDisembodied = nullptr);

    /**
     * @brief Get the dragon's fire projectile
     * @return Reference to the fire projectile
     */
    Fire &getFire();

    /**
     * @brief Get the dragon's fire projectile (const version)
     * @return Const reference to the fire projectile
     */
    const Fire &getFire() const;

    /**
     * @brief Check if dragon can breathe fire
     * @return true if dragon can breathe fire
     */
    bool canBreatheFire() const;

    /**
     * @brief Breathe fire in the direction towards the player
     * @param playerPos Player position
     * @return true if fire was breathed successfully
     */
    bool breatheFire(Vector2 playerPos);

private:
    std::unique_ptr<Fire> fireProjectile;         // Dragon's fire projectile
    float fireBreathCooldown;                     // Cooldown timer for breathing fire
    float fireBreathRange;                        // Maximum range for breathing fire
    static const float FIRE_BREATH_COOLDOWN_TIME; // Cooldown duration between fire breaths
    static const float FIRE_BREATH_RANGE;         // Maximum range to breathe fire

    /**
     * @brief Find the best direction to move toward the player using smart pathfinding
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @return Best direction to move
     */
    Direction findSmartDirectionToPlayer(const Player &player, const Grid &grid);

    /**
     * @brief Find a tactical position for better fire breathing opportunities
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @return Direction to move for tactical advantage
     */
    Direction findTacticalPosition(const Player &player, const Grid &grid);

    /**
     * @brief Find a random valid direction to move
     * @param grid Reference to the game grid
     * @return Random valid direction
     */
    Direction findRandomValidDirection(const Grid &grid);

    /**
     * @brief Check if player is within fire breathing range
     * @param playerPos Player position
     * @return true if player is within range
     */
    bool isPlayerInFireRange(Vector2 playerPos) const;

    /**
     * @brief Check if there's a direct tunnel path to the player for fire breathing
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @return true if there's a clear tunnel path
     */
    bool hasDirectTunnelPathToPlayer(const Player &player, const Grid &grid) const;

    /**
     * @brief Calculate direction to breathe fire towards player
     * @param playerPos Player position
     * @return Direction to breathe fire
     */
    Direction calculateFireDirection(Vector2 playerPos) const;

    /**
     * @brief Update fire breath cooldown
     */
    void updateFireBreathCooldown();
};

#endif // GREEN_DRAGON_H