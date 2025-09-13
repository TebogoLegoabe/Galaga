#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <raylib-cpp.hpp>
#include "GameEnums.h"

/**
 * @brief Abstract base class for all game objects
 */
class GameObject
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

protected:
    Vector2 position; // Current position of the object
    Vector2 size;     // Size of the object
    bool active;      // Whether the object is active

    /**
     * @brief Check if the object is within screen bounds
     * @param screenWidth Width of the screen
     * @param screenHeight Height of the screen
     * @return true if within bounds, false otherwise
     */
    bool isWithinBounds(int screenWidth, int screenHeight) const;
};

#endif // GAME_OBJECT_H