#ifndef MONSTER_H
#define MONSTER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Abstract base class for all monsters
 */
class Monster : public GameObject
{
public:
    /**
     * @brief Constructor for Monster
     * @param startPos Starting position in world coordinates
     */
    Monster(Vector2 startPos = {0, 0});

    /**
     * @brief Virtual destructor
     */
    virtual ~Monster() = default;

    /**
     * @brief Update the monster (pure virtual)
     */
    void update() override = 0;

    /**
     * @brief Draw the monster (pure virtual)
     */
    void draw() override = 0;

    /**
     * @brief Move the monster in a direction
     * @param direction Direction to move
     * @param grid Reference to the game grid
     * @return true if movement was successful
     */
    virtual bool move(Direction direction, Grid &grid);

    /**
     * @brief Get the monster's current state
     * @return Current MonsterState
     */
    MonsterState getState() const;

    /**
     * @brief Set the monster's state
     * @param newState New monster state
     */
    void setState(MonsterState newState);

    /**
     * @brief Check if monster can move to a position
     * @param newPos Position to check in world coordinates
     * @param grid Reference to the game grid
     * @return true if the position is valid
     */
    bool canMoveTo(Vector2 newPos, const Grid &grid) const;

    /**
     * @brief Get the monster's grid position
     * @param grid Reference to the game grid
     * @return Monster position in grid coordinates
     */
    Vector2 getGridPosition(const Grid &grid) const;

    /**
     * @brief Reset the monster to starting position
     * @param startPos Starting position
     */
    virtual void reset(Vector2 startPos);

    /**
     * @brief Get the monster's speed
     * @return Movement speed in pixels per frame
     */
    float getSpeed() const;

    /**
     * @brief Set target position for AI movement
     * @param target Target position in world coordinates
     */
    void setTargetPosition(Vector2 target);

protected:
    MonsterState state;     // Current monster state
    float speed;            // Movement speed in pixels per frame
    Vector2 targetPosition; // Target position for AI
    Vector2 lastPosition;   // Last valid position
    float moveTimer;        // Timer for movement decisions

    /**
     * @brief Check if a world position is within valid bounds
     * @param worldPos Position to check
     * @param grid Reference to the game grid
     * @return true if within bounds
     */
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;

    /**
     * @brief Calculate direction to target
     * @param target Target position
     * @return Direction to move towards target
     */
    Direction getDirectionToTarget(Vector2 target) const;
};

#endif // MONSTER_H