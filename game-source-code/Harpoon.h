#ifndef HARPOON_H
#define HARPOON_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>
#include <cmath>

/**
 * @brief Enumeration for harpoon states
 */
enum class HarpoonState
{
    FLYING,    ///< Harpoon is flying through the air
    INFLATING, ///< Harpoon hit a monster and is inflating it
    INACTIVE   ///< Harpoon is not active
};

/**
 * @brief Harpoon projectile class
 */
class Harpoon : public GameObject
{
public:
    /**
     * @brief Constructor for Harpoon
     * @param startPos Starting position in world coordinates
     * @param direction Direction the harpoon is fired
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
     * @brief Fire the harpoon in a direction
     * @param startPos Starting position
     * @param fireDirection Direction to fire
     */
    void fire(Vector2 startPos, Direction fireDirection);

    /**
     * @brief Get the harpoon's current state
     * @return Current HarpoonState
     */
    HarpoonState getState() const;

    /**
     * @brief Set the harpoon's state
     * @param newState New harpoon state
     */
    void setState(HarpoonState newState);

    /**
     * @brief Get the direction the harpoon is traveling
     * @return Current direction
     */
    Direction getDirection() const;

    /**
     * @brief Check if harpoon is within grid bounds
     * @param grid Reference to the game grid
     * @return true if within bounds
     */
    bool isWithinBounds(const Grid &grid) const;

    /**
     * @brief Reset the harpoon to inactive state
     */
    void reset();

private:
    Direction direction;             ///< Direction the harpoon is traveling
    HarpoonState state;              ///< Current harpoon state
    float speed;                     ///< Harpoon movement speed
    float inflateTimer;              ///< Timer for inflation process
    static const float INFLATE_TIME; ///< Time to inflate monster (seconds)

    /**
     * @brief Update flying movement
     */
    void updateFlying();

    /**
     * @brief Update inflation process
     */
    void updateInflating();
};

#endif // HARPOON_H