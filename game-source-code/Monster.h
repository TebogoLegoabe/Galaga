#ifndef MONSTER_H
#define MONSTER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include <raylib-cpp.hpp>

/**
 * @brief Monster class for underground creatures
 */
class Monster : public GameObject
{
public:
    /**
     * @brief Constructor for Monster
     * @param startPos Starting position in world coordinates
     * @param monsterType Type of monster (red or green)
     */
    Monster(Vector2 startPos = {0, 0}, bool isGreen = false);

    /**
     * @brief Virtual destructor
     */
    virtual ~Monster() = default;

    /**
     * @brief Update the monster
     */
    virtual void update() override;

    /**
     * @brief Draw the monster
     */
    virtual void draw() override;

    /**
     * @brief Update monster AI and movement
     * @param grid Reference to the game grid
     * @param playerPos Player's current position
     */
    void updateAI(const Grid &grid, Vector2 playerPos);

    /**
     * @brief Check if the monster can move to a position
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
     * @brief Get the monster's current state
     * @return Current MonsterState
     */
    MonsterState getState() const;

    /**
     * @brief Kill the monster
     */
    void kill();

    /**
     * @brief Check if monster is dead
     * @return true if monster is dead
     */
    bool isDead() const;

    /**
     * @brief Get the monster's speed
     * @return Movement speed in pixels per frame
     */
    float getSpeed() const;

    /**
     * @brief Set the monster's speed
     * @param newSpeed New movement speed
     */
    void setSpeed(float newSpeed);

    /**
     * @brief Reset the monster to starting position and state
     * @param startPos Starting position
     */
    void reset(Vector2 startPos);

    /**
     * @brief Check if this is a green dragon
     * @return true if green dragon, false if red monster
     */
    bool isGreenDragon() const;

protected:
    MonsterState state;        // Current monster state
    Direction facingDirection; // Direction the monster is facing
    float speed;               // Movement speed in pixels per frame
    Vector2 targetPosition;    // Target position for smooth movement
    bool isMoving;             // Whether the monster is currently moving
    bool greenDragon;          // Whether this is a green dragon
    float moveTimer;           // Timer for controlling movement frequency
    float moveInterval;        // How often the monster tries to move (in seconds)

    // Protected helper methods for derived classes
    void updateMovement();
    Direction chooseNextDirection(const Grid &grid, Vector2 playerPos);
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
    float getDistanceToPlayer(Vector2 playerPos) const;
    std::vector<Direction> getValidDirections(const Grid &grid) const;
    Direction getDirectionTowardsPlayer(Vector2 playerPos) const;
};

#endif // MONSTER_H