#include "enemy.h"

Enemy::Enemy(Vector2 pos) : position(pos) {}

void Enemy::update() { position.y += speed; }

void Enemy::draw() const { DrawRectangleV(position, {size, size}, RED); }

bool Enemy::isOffScreen() const { return position.y > GetScreenHeight(); }

Rectangle Enemy::getRect() const { return {position.x, position.y, size, size}; }

