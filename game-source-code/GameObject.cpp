#include "GameObject.h"

GameObject::GameObject(Vector2 pos, Vector2 objSize)
    : position(pos), size(objSize), active(true)
{
}

Rectangle GameObject::getBounds() const
{
    return Rectangle{position.x, position.y, size.x, size.y};
}

bool GameObject::isActive() const
{
    return active;
}

void GameObject::setActive(bool active)
{
    this->active = active;
}

Vector2 GameObject::getPosition() const
{
    return position;
}

void GameObject::setPosition(Vector2 newPos)
{
    position = newPos;
}

Vector2 GameObject::getSize() const
{
    return size;
}

bool GameObject::isWithinBounds(int screenWidth, int screenHeight) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x + size.x <= screenWidth &&
           position.y + size.y <= screenHeight;
}
