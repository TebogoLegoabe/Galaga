#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Possible states for a rock
 */
enum class RockState
{
    STATIONARY, // Rock is sitting still
    FALLING,    // Rock is falling
    FALLEN      // Rock has fallen and stopped
};

/**
 * @brief Rock class that can fall when dug under
 */
class Rock : public GameObject
{
public:
    /**
     * @brief Constructor for Rock
     * @param startPos Starting position in world coordinates
     */
    Rock(Vector2 startPos = {0, 0});
    /**
     * @brief Update the rock
     */
    void update() override;
    /**
     * @brief Draw the rock
     */
    void draw() override;
    /**
     * @brief Check if rock should start falling
     * @param grid Reference to the game grid
     */
    void checkShouldFall(const Grid &grid);
    /**
     * @brief Get the rock's current state
     * @return Current RockState
     */
    RockState getState() const;
    /**
     * @brief Get the rock's grid position
     * @param grid Reference to the game grid
     * @return Rock position in grid coordinates
     */
    Vector2 getGridPosition(const Grid &grid) const;
    /**
     * @brief Check if rock is falling
     * @return true if rock is falling
     */
    bool isFalling() const;
    /**
     * @brief Reset the rock to starting position and state
     * @param startPos Starting position
     */
    void reset(Vector2 startPos);
    /**
     * @brief Get the rock's speed
     * @return Fall speed in pixels per frame
     */
    float getSpeed() const;
    /**
     * @brief Set the rock's speed
     * @param newSpeed New fall speed
     */
    void setSpeed(float newSpeed);

private:
    RockState currentState;             // Current rock state
    float speed;                        // Fall speed in pixels per frame
    float fallDelay;                    // Delay before starting to fall
    float fallTimer;                    // Timer for fall delay
    Vector2 originalPosition;           // Original position when placed
    static const float FALL_DELAY_TIME; // How long to wait before falling
    static const float FALL_SPEED;      // How fast rocks fall
    /**
     * @brief Update falling movement
     * @param grid Reference to the game grid
     */
    void updateFalling(const Grid &grid);
    /**
     * @brief Check if there's solid ground below
     * @param grid Reference to the game grid
     * @return true if there's solid ground below
     */
    bool hasGroundBelow(const Grid &grid) const;
};
#endif // ROCK_H