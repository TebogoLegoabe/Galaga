#include "bullet.h"

Bullet::Bullet(Vector2 pos) : position(pos) {}

void Bullet::update() { position.y += speed; }

void Bullet::draw() const { DrawRectangleV(position, {width, height}, YELLOW); }

bool Bullet::isOffScreen() const { return position.y + height < 0; }

Rectangle Bullet::getRect() const { return {position.x, position.y, width, height}; }

