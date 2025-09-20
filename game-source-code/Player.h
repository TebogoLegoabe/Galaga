#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include "Harpoon.h"
#include <raylib-cpp.hpp>
#include <memory>

/**
 * @brief Player character class for Dig Dug
 */
class Player : public GameObject
{
public:
    /**
     * @brief Constructor for Player
     * @param startPos Starting position in world coordinates
     */
    Player(Vector2 startPos = {0, 0});

    /**
     * @brief Update the player
     */
    void update() override;

    /**
     * @brief Draw the player
     */
    void draw() override;

    /**
     * @brief Move the player in a direction
     * @param direction Direction to move
     * @param grid Reference to the game grid
     * @return true if movement was successful
     */
    bool move(Direction direction, Grid &grid);

    /**
     * @brief Shoot the harpoon in the facing direction
     * @return true if harpoon was fired successfully
     */
    bool shoot();

    /**
     * @brief Get the direction the player is facing
     * @return Current facing direction
     */
    Direction getFacingDirection() const;

    /**
     * @brief Check if the player can move to a position
     * @param newPos Position to check in world coordinates
     * @param grid Reference to the game grid
     * @return true if the position is valid
     */
    bool canMoveTo(Vector2 newPos, const Grid &grid) const;

    /**
     * @brief Get the player's grid position
     * @param grid Reference to the game grid
     * @return Player position in grid coordinates
     */
    Vector2 getGridPosition(const Grid &grid) const;

    /**
     * @brief Reset the player to starting position
     * @param startPos Starting position
     */
    void reset(Vector2 startPos);

    /**
     * @brief Get the player's speed
     * @return Movement speed in pixels per frame
     */
    float getSpeed() const;

    /**
     * @brief Set the player's speed
     * @param newSpeed New movement speed
     */
    void setSpeed(float newSpeed);

    /**
     * @brief Get the player's harpoon
     * @return Reference to the harpoon
     */
    Harpoon &getHarpoon();

    /**
     * @brief Get the player's harpoon (const version)
     * @return Const reference to the harpoon
     */
    const Harpoon &getHarpoon() const;

    /**
     * @brief Check if player can currently shoot
     * @return true if player can shoot
     */
    bool canShoot() const;

private:
    Direction facingDirection;              // Direction the player is facing
    float speed;                            // Movement speed in pixels per frame
    Vector2 targetPosition;                 // Target position for smooth movement
    bool isMoving;                          // Whether the player is currently moving
    std::unique_ptr<Harpoon> harpoon;       // Player's harpoon weapon
    float shootCooldown;                    // Cooldown timer for shooting
    static const float SHOOT_COOLDOWN_TIME; // Cooldown duration

    void updateMovement();
    void updateShooting();
    void digAtCurrentPosition(Grid &grid);
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
};

#endif // PLAYER_H