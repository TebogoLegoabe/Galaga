#ifndef MOVABLE_H
#define MOVABLE_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"
#include "Grid.h"

/**
 * @brief Interface for movable game objects
 */
class Movable
{
public:
    virtual ~Movable() = default;

    /**
     * @brief Move in a direction
     * @param direction Direction to move
     * @param grid Game grid reference
     * @return true if movement successful
     */
    virtual bool move(Direction direction, Grid &grid) = 0;

    /**
     * @brief Check if can move to position
     * @param newPos Target position
     * @param grid Game grid reference
     * @return true if position is valid
     */
    virtual bool canMoveTo(Vector2 newPos, const Grid &grid) const = 0;

    /**
     * @brief Get grid position
     * @param grid Game grid reference
     * @return Grid coordinates
     */
    virtual Vector2 getGridPosition(const Grid &grid) const = 0;

    /**
     * @brief Get movement speed
     * @return Speed in pixels per frame
     */
    virtual float getSpeed() const = 0;

    /**
     * @brief Set movement speed
     * @param newSpeed Speed value
     */
    virtual void setSpeed(float newSpeed) = 0;

protected:
    /**
     * @brief Update movement (shared logic)
     */
    virtual void updateMovement() = 0;
};

#endif // MOVABLE_H