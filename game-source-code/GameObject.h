#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"
#include "Movable.h"

/**
 * @brief Abstract base class for all game objects
 */
class GameObject : public Movable
{
public:
    /**
     * @brief Constructor for GameObject
     * @param pos Initial position
     * @param objSize Size of the object
     */
    GameObject(Vector2 pos = {0, 0}, Vector2 objSize = {32, 32});

    /**
     * @brief Virtual destructor
     */
    virtual ~GameObject() = default;

    /**
     * @brief Pure virtual update method
     */
    virtual void update() = 0;

    /**
     * @brief Pure virtual draw method
     */
    virtual void draw() = 0;

    /**
     * @brief Get the bounding rectangle for collision detection
     * @return Rectangle representing the object's bounds
     */
    Rectangle getBounds() const;

    /**
     * @brief Check if the object is active
     * @return true if active, false otherwise
     */
    bool isActive() const;

    /**
     * @brief Set the active state of the object
     * @param active New active state
     */
    void setActive(bool active);

    /**
     * @brief Get the current position
     * @return Current position as Vector2
     */
    Vector2 getPosition() const;

    /**
     * @brief Set the position
     * @param newPos New position
     */
    void setPosition(Vector2 newPos);

    /**
     * @brief Get the size of the object
     * @return Size as Vector2
     */
    Vector2 getSize() const;

    // Movable interface implementation
    bool move(Direction direction, Grid &grid) override;
    bool canMoveTo(Vector2 newPos, const Grid &grid) const override;
    Vector2 getGridPosition(const Grid &grid) const override;
    float getSpeed() const override;
    void setSpeed(float newSpeed) override;

protected:
    Vector2 position;       // Current position
    Vector2 size;           // Size of the object
    bool active;            // Active state
    float speed;            // Movement speed
    Vector2 targetPosition; // Target for smooth movement
    bool isMoving;          // Movement state

    void updateMovement() override;
    bool isWithinBounds(int screenWidth, int screenHeight) const;
    bool isWithinGridBounds(Vector2 worldPos, const Grid &grid) const;
};

#endif // GAME_OBJECT_H