#ifndef HARPOON_H
#define HARPOON_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Harpoon projectile fired by the player
 */
class Harpoon : public GameObject
{
public:
    /**
     * @brief Constructor for Harpoon
     * @param startPos Starting position in world coordinates
     * @param direction Direction the harpoon travels
     */
    Harpoon(Vector2 startPos = {0, 0}, Direction direction = Direction::RIGHT);

    /**
     * @brief Update the harpoon
     */
    void update() override;

    /**
     * @brief Draw the harpoon
     */
    void draw() override;

    /**
     * @brief Fire the harpoon from a position in a direction
     * @param startPos Starting position
     * @param dir Direction to fire
     */
    void fire(Vector2 startPos, Direction dir);

    /**
     * @brief Get the harpoon's direction
     * @return Current direction
     */
    Direction getDirection() const;

    /**
     * @brief Check if harpoon is currently active/fired
     * @return true if active
     */
    bool isHarpoonActive() const;

    /**
     * @brief Deactivate the harpoon
     */
    void deactivate();

    /**
     * @brief Check if harpoon should be destroyed (hit wall, out of bounds, etc.)
     * @param grid Reference to the game grid
     * @return true if harpoon should be destroyed
     */
    bool shouldDestroy(const Grid &grid) const;

    /**
     * @brief Reset the harpoon to inactive state
     */
    void reset();

    /**
     * @brief Get the harpoon's speed
     * @return Movement speed
     */
    float getSpeed() const;

    /**
     * @brief Set the harpoon's speed
     * @param newSpeed New movement speed
     */
    void setSpeed(float newSpeed);

private:
    Direction direction;  // Direction the harpoon is traveling
    float speed;          // Movement speed
    float maxRange;       // Maximum travel distance
    float travelDistance; // Distance traveled so far
    bool fired;           // Whether the harpoon has been fired

    /**
     * @brief Update harpoon movement
     */
    void updateMovement();

    /**
     * @brief Check if harpoon is within valid bounds
     * @param grid Reference to the game grid
     * @return true if within bounds
     */
    bool isWithinBounds(const Grid &grid) const;
};

#endif // HARPOON_H