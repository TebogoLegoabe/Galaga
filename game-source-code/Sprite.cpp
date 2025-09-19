#include "Sprite.h"

void Sprite::drawDigDug(Vector2 position, Vector2 size, Direction direction)
{
    switch (direction)
    {
    case Direction::RIGHT:
        drawDigDugRight(position, size);
        break;
    case Direction::LEFT:
        drawDigDugLeft(position, size);
        break;
    case Direction::UP:
        drawDigDugUp(position, size);
        break;
    case Direction::DOWN:
        drawDigDugDown(position, size);
        break;
    default:
        drawDigDugRight(position, size); // Default to right
        break;
    }
}

void Sprite::drawRock(Vector2 position, Vector2 size)
{
    // Draw a detailed rock sprite
    float w = size.x;
    float h = size.y;

    // Main rock body
    DrawRectangle(position.x + 2, position.y + 2, w - 4, h - 4, GRAY);

    // Rock highlights and shadows for 3D effect
    DrawRectangle(position.x + 1, position.y + 1, w - 2, 3, LIGHTGRAY);
    DrawRectangle(position.x + 1, position.y + 1, 3, h - 2, LIGHTGRAY);
    DrawRectangle(position.x + w - 4, position.y + 3, 3, h - 6, DARKGRAY);
    DrawRectangle(position.x + 3, position.y + h - 4, w - 6, 3, DARKGRAY);

    // Rock texture details
    DrawPixel(position.x + 5, position.y + 7, DARKGRAY);
    DrawPixel(position.x + 12, position.y + 5, DARKGRAY);
    DrawPixel(position.x + 8, position.y + 15, DARKGRAY);
    DrawPixel(position.x + 18, position.y + 12, DARKGRAY);
}

void Sprite::drawDigDugRight(Vector2 pos, Vector2 size)
{
    float w = size.x;
    float h = size.y;

    // Body (blue jumpsuit)
    DrawRectangle(pos.x + 6, pos.y + 8, 16, 12, BLUE);

    // Head
    DrawCircle(pos.x + 14, pos.y + 8, 6, Color{255, 220, 177, 255}); // Skin tone

    // Hair (black)
    DrawCircle(pos.x + 14, pos.y + 5, 4, BLACK);

    // Eyes
    DrawPixel(pos.x + 16, pos.y + 7, BLACK);
    DrawPixel(pos.x + 17, pos.y + 7, BLACK);

    // Nose
    DrawPixel(pos.x + 18, pos.y + 8, Color{200, 150, 120, 255});

    // Arms
    DrawRectangle(pos.x + 22, pos.y + 10, 4, 6, Color{255, 220, 177, 255}); // Right arm
    DrawRectangle(pos.x + 4, pos.y + 12, 4, 4, Color{255, 220, 177, 255});  // Left arm

    // Legs
    DrawRectangle(pos.x + 8, pos.y + 20, 4, 6, BLUE);  // Left leg
    DrawRectangle(pos.x + 16, pos.y + 20, 4, 6, BLUE); // Right leg

    // Feet
    DrawRectangle(pos.x + 6, pos.y + 26, 6, 2, BLACK);  // Left foot
    DrawRectangle(pos.x + 16, pos.y + 26, 6, 2, BLACK); // Right foot

    // Harpoon gun (pointing right)
    DrawRectangle(pos.x + 26, pos.y + 12, 4, 2, DARKGRAY);
    DrawPixel(pos.x + 30, pos.y + 13, RED); // Harpoon tip
}

void Sprite::drawDigDugLeft(Vector2 pos, Vector2 size)
{
    float w = size.x;
    float h = size.y;

    // Body (blue jumpsuit)
    DrawRectangle(pos.x + 6, pos.y + 8, 16, 12, BLUE);

    // Head
    DrawCircle(pos.x + 14, pos.y + 8, 6, Color{255, 220, 177, 255}); // Skin tone

    // Hair (black)
    DrawCircle(pos.x + 14, pos.y + 5, 4, BLACK);

    // Eyes
    DrawPixel(pos.x + 11, pos.y + 7, BLACK);
    DrawPixel(pos.x + 12, pos.y + 7, BLACK);

    // Nose
    DrawPixel(pos.x + 10, pos.y + 8, Color{200, 150, 120, 255});

    // Arms
    DrawRectangle(pos.x + 2, pos.y + 10, 4, 6, Color{255, 220, 177, 255});  // Left arm
    DrawRectangle(pos.x + 20, pos.y + 12, 4, 4, Color{255, 220, 177, 255}); // Right arm

    // Legs
    DrawRectangle(pos.x + 8, pos.y + 20, 4, 6, BLUE);  // Left leg
    DrawRectangle(pos.x + 16, pos.y + 20, 4, 6, BLUE); // Right leg

    // Feet
    DrawRectangle(pos.x + 6, pos.y + 26, 6, 2, BLACK);  // Left foot
    DrawRectangle(pos.x + 16, pos.y + 26, 6, 2, BLACK); // Right foot

    // Harpoon gun (pointing left)
    DrawRectangle(pos.x - 2, pos.y + 12, 4, 2, DARKGRAY);
    DrawPixel(pos.x - 3, pos.y + 13, RED); // Harpoon tip
}

void Sprite::drawDigDugUp(Vector2 pos, Vector2 size)
{
    float w = size.x;
    float h = size.y;

    // Body (blue jumpsuit)
    DrawRectangle(pos.x + 6, pos.y + 8, 16, 12, BLUE);

    // Head
    DrawCircle(pos.x + 14, pos.y + 8, 6, Color{255, 220, 177, 255}); // Skin tone

    // Hair (black)
    DrawCircle(pos.x + 14, pos.y + 5, 4, BLACK);

    // Eyes
    DrawPixel(pos.x + 12, pos.y + 6, BLACK);
    DrawPixel(pos.x + 16, pos.y + 6, BLACK);

    // Nose
    DrawPixel(pos.x + 14, pos.y + 8, Color{200, 150, 120, 255});

    // Arms (spread for balance)
    DrawRectangle(pos.x + 2, pos.y + 10, 6, 4, Color{255, 220, 177, 255});  // Left arm
    DrawRectangle(pos.x + 20, pos.y + 10, 6, 4, Color{255, 220, 177, 255}); // Right arm

    // Legs
    DrawRectangle(pos.x + 8, pos.y + 20, 4, 6, BLUE);  // Left leg
    DrawRectangle(pos.x + 16, pos.y + 20, 4, 6, BLUE); // Right leg

    // Feet
    DrawRectangle(pos.x + 6, pos.y + 26, 6, 2, BLACK);  // Left foot
    DrawRectangle(pos.x + 16, pos.y + 26, 6, 2, BLACK); // Right foot

    // Harpoon gun (pointing up)
    DrawRectangle(pos.x + 13, pos.y + 2, 2, 4, DARKGRAY);
    DrawPixel(pos.x + 14, pos.y + 1, RED); // Harpoon tip
}

void Sprite::drawDigDugDown(Vector2 pos, Vector2 size)
{
    float w = size.x;
    float h = size.y;

    // Body (blue jumpsuit)
    DrawRectangle(pos.x + 6, pos.y + 8, 16, 12, BLUE);

    // Head (tilted down)
    DrawCircle(pos.x + 14, pos.y + 8, 6, Color{255, 220, 177, 255}); // Skin tone

    // Hair (black)
    DrawCircle(pos.x + 14, pos.y + 5, 4, BLACK);

    // Eyes (looking down)
    DrawPixel(pos.x + 12, pos.y + 9, BLACK);
    DrawPixel(pos.x + 16, pos.y + 9, BLACK);

    // Nose
    DrawPixel(pos.x + 14, pos.y + 10, Color{200, 150, 120, 255});

    // Arms
    DrawRectangle(pos.x + 2, pos.y + 12, 6, 4, Color{255, 220, 177, 255});  // Left arm
    DrawRectangle(pos.x + 20, pos.y + 12, 6, 4, Color{255, 220, 177, 255}); // Right arm

    // Legs
    DrawRectangle(pos.x + 8, pos.y + 20, 4, 6, BLUE);  // Left leg
    DrawRectangle(pos.x + 16, pos.y + 20, 4, 6, BLUE); // Right leg

    // Feet
    DrawRectangle(pos.x + 6, pos.y + 26, 6, 2, BLACK);  // Left foot
    DrawRectangle(pos.x + 16, pos.y + 26, 6, 2, BLACK); // Right foot

    // Harpoon gun (pointing down)
    DrawRectangle(pos.x + 13, pos.y + 22, 2, 4, DARKGRAY);
    DrawPixel(pos.x + 14, pos.y + 26, RED); // Harpoon tip
}