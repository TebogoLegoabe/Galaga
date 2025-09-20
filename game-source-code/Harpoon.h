#ifndef HARPOON_H
#define HARPOON_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Harpoon projectile class for Dig Dug
 */
class Harpoon : public GameObject
{
public:
    /**
     * @brief Constructor for Harpoon
     * @param startPos Starting position in world coordinates
     * @param direction Direction the harpoon is fired
     */
    Harpoon(Vector2 startPos = {0, 0}, Direction dir = Direction::RIGHT);

    /**
     * @brief Update the harpoon
     */
    void update() override;

    /**
     * @brief Draw the harpoon
     */
    void draw() override;

    /**
     * @brief Fire the harpoon
     * @param startPos Starting position
     * @param direction Direction to fire
     */
    void fire(Vector2 startPos, Direction direction);

    /**
     * @brief Check if harpoon is active (fired and moving)
     * @return true if harpoon is active
     */
    bool isHarpoonActive() const;

    /**
     * @brief Stop the harpoon (hit something or max range)
     */
    void stop();

    /**
     * @brief Get the direction the harpoon is traveling
     * @return Current direction
     */
    Direction getDirection() const;

    /**
     * @brief Get the harpoon's speed
     * @return Movement speed in pixels per frame
     */
    float getSpeed() const;

    /**
     * @brief Check if harpoon has reached maximum range
     * @param grid Reference to the game grid
     * @return true if at max range or hit obstacle
     */
    bool hasReachedMaxRange(const Grid &grid) const;

    /**
     * @brief Reset the harpoon to inactive state
     */
    void reset();

    /**
     * @brief Get if harpoon is active (public accessor)
     * @return true if harpoon is active
     */
    bool getIsActive() const;

private:
    Direction direction;   // Direction the harpoon is traveling
    float speed;           // Movement speed in pixels per frame
    Vector2 startPosition; // Starting position for range calculation
    float maxRange;        // Maximum range in pixels
    bool isActive;         // Whether the harpoon is currently active

    /**
     * @brief Check if harpoon can continue in current direction
     * @param grid Reference to the game grid
     * @return true if path is clear
     */
    bool canContinue(const Grid &grid) const;
};

#endif // HARPOON_H