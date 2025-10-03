#ifndef FIRE_H
#define FIRE_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Fire projectile breathed by green dragons
 */
class Fire : public GameObject
{
public:
    /**
     * @brief Constructor for Fire
     * @param startPos Starting position in world coordinates
     * @param direction Direction the fire travels
     */
    Fire(Vector2 startPos = {0, 0}, Direction direction = Direction::RIGHT);

    /**
     * @brief Update the fire projectile
     */
    void update() override;

    /**
     * @brief Draw the fire projectile
     */
    void draw() override;

    /**
     * @brief Breathe fire from a position in a direction
     * @param startPos Starting position
     * @param dir Direction to breathe fire
     */
    void breathe(Vector2 startPos, Direction dir);

    /**
     * @brief Get the fire's direction
     * @return Current direction
     */
    Direction getDirection() const;

    /**
     * @brief Check if fire is currently active/breathing
     * @return true if active
     */
    bool isFireActive() const;

    /**
     * @brief Deactivate the fire
     */
    void deactivate();

    /**
     * @brief Check if fire should be destroyed (hit wall, out of bounds, etc.)
     * @param grid Reference to the game grid
     * @return true if fire should be destroyed
     */
    bool shouldDestroy(const Grid &grid) const;

    /**
     * @brief Reset the fire to inactive state
     */
    void reset();

    /**
     * @brief Get the fire's speed
     * @return Movement speed
     */
    float getSpeed() const;

    /**
     * @brief Set the fire's speed
     * @param newSpeed New movement speed
     */
    void setSpeed(float newSpeed);

    /**
     * @brief Get the remaining burn time
     * @return Burn time left in seconds
     */
    float getBurnTime() const;

private:
    Direction direction;  // Direction the fire is traveling
    float speed;          // Movement speed
    float maxRange;       // Maximum travel distance
    float travelDistance; // Distance traveled so far
    float burnTime;       // How long the fire lasts
    float maxBurnTime;    // Maximum burn duration
    bool breathing;       // Whether the fire has been breathed
    float animationTimer; // Timer for fire animation effects

    /**
     * @brief Update fire movement
     */
    void updateMovement();

    /**
     * @brief Update burn time
     */
    void updateBurnTime();

    /**
     * @brief Check if fire is within valid bounds
     * @param grid Reference to the game grid
     * @return true if within bounds
     */
    bool isWithinBounds(const Grid &grid) const;

    /**
     * @brief Update fire animation
     */
    void updateAnimation();
};

#endif // FIRE_H