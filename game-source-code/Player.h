#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

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

private:
    Direction facingDirection; // Direction the player is facing
    float speed;               // Movement speed in pixels per frame
    Vector2 targetPosition;    // Target position for smooth movement
    bool isMoving;             // Whether the player is currently moving

    /**
     * @brief Update smooth movement towards target
     */
    void updateMovement();

    /**
     * @brief Dig a tunnel at the player's current position
     * @param grid Reference to the game grid
     */
    void digAtCurrentPosition(Grid &grid);

    /**
     * @brief Check if a world position is within valid bounds
     * @param worldPos Position to check
     * @param grid Reference to the game grid
     * @return true if within bounds
     */
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
};

#endif // PLAYER_H