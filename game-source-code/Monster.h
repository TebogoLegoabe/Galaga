#ifndef MONSTER_H
#define MONSTER_H

#include "GameObject.h"
#include "GameEnums.h"
#include "Grid.h"
#include "Player.h"
#include <raylib-cpp.hpp>
#include <functional>

/**
 * @brief Monster class for Dig Dug enemies
 */
class Monster : public GameObject
{
public:
    /**
     * @brief Constructor for Monster
     * @param startPos Starting position in world coordinates
     * @param state Initial monster state
     */
    Monster(Vector2 startPos = {0, 0}, MonsterState state = MonsterState::IN_TUNNEL);

    /**
     * @brief Update the monster
     */
    void update() override;

    /**
     * @brief Draw the monster
     */
    void draw() override;

    /**
     * @brief Update monster AI to chase the player
     * @param player Reference to the player
     * @param grid Reference to the game grid
     * @param canBecomeDisembodied Whether the monster is allowed to become disembodied
     * @param notifyDisembodied Callback function to notify when monster becomes disembodied
     */
    void updateAI(const Player &player, const Grid &grid, bool canBecomeDisembodied, std::function<void()> notifyDisembodied = nullptr);

    /**
     * @brief Move the monster in a direction
     * @param direction Direction to move
     * @param grid Reference to the game grid
     * @return true if movement was successful
     */
    bool move(Direction direction, const Grid &grid);

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
     * @brief Get the monster's grid position
     * @param grid Reference to the game grid
     * @return Monster position in grid coordinates
     */
    Vector2 getGridPosition(const Grid &grid) const;

    /**
     * @brief Check if the monster can move to a position
     * @param newPos Position to check in world coordinates
     * @param grid Reference to the game grid
     * @return true if the position is valid for the monster's current state
     */
    bool canMoveTo(Vector2 newPos, const Grid &grid) const;

    /**
     * @brief Reset the monster to starting position and state
     * @param startPos Starting position
     * @param state Starting state
     */
    void reset(Vector2 startPos, MonsterState state = MonsterState::IN_TUNNEL);

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
     * @brief Check if the monster is dead
     * @return true if monster is dead
     */
    bool isDead() const;

protected:
    /**
     * @brief Update movement (protected so derived classes can access)
     */
    void updateMovement();

    /**
     * @brief Update state timer (protected so derived classes can access)
     */
    void updateStateTimer();

    bool shouldBecomeDisembodied(const Player &player, const Grid &grid);
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
    float calculateDistanceToPlayer(const Player &player) const;
    bool isPlayerInSameTunnel(const Player &player, const Grid &grid) const;

    MonsterState currentState; // Current monster state (protected for derived classes)
    float speed;               // Movement speed in pixels per frame
    Vector2 targetPosition;    // Target position for smooth movement
    bool isMoving;             // Whether the monster is currently moving
    float stateTimer;          // Timer for state transitions
    float aiUpdateTimer;       // Timer for AI updates
    Direction lastDirection;   // Last movement direction

private:
    // AI and pathfinding methods
    Direction findBestDirectionToPlayer(const Player &player, const Grid &grid);
    Direction findRandomValidDirection(const Grid &grid);
};

#endif // MONSTER_H