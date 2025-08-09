#include "player.h"

Player::Player() {
    Image img = GenImageColor(32, 32, BLANK);
    Vector2 p1{16, 0};
    Vector2 p2{0, 32};
    Vector2 p3{32, 32};
    ImageDrawTriangle(&img, p1, p2, p3, BLUE);
    texture = LoadTextureFromImage(img);
    UnloadImage(img);

    position = {(float)(GetScreenWidth() / 2 - texture.width / 2),
                (float)(GetScreenHeight() - texture.height - 10)};
}

Player::~Player() { UnloadTexture(texture); }

void Player::update(std::vector<Bullet> &bullets) {
    if (IsKeyDown(KEY_LEFT))
        position.x -= speed;
    if (IsKeyDown(KEY_RIGHT))
        position.x += speed;

    if (position.x < 0)
        position.x = 0;
    if (position.x > GetScreenWidth() - texture.width)
        position.x = GetScreenWidth() - texture.width;

    if (IsKeyPressed(KEY_SPACE)) {
        Vector2 bulletPos{position.x + texture.width / 2 - 2.5f, position.y};
        bullets.emplace_back(bulletPos);
    }
}

void Player::draw() const { DrawTextureV(texture, position, WHITE); }

Rectangle Player::getRect() const {
    return {position.x, position.y, (float)texture.width, (float)texture.height};
}

