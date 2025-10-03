#include "Sprite.h"
#include <cmath>

Sprite::Sprite(Vector2 pos, Vector2 spriteSize)
    : position(pos), size(spriteSize)
{
}

void Sprite::drawDigDug(Vector2 position, Direction direction, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw Dig Dug's body (blue with white details)
    DrawCircleV(center, size.x / 2 - 2, BLUE);

    // Draw helmet/head details
    DrawCircleV({center.x, center.y - 3}, size.x / 3, LIGHTGRAY);

    // Draw eyes
    drawEyes(center, 2.0f, BLACK);

    // Draw drill/tool based on direction
    Vector2 drillPos = center;
    switch (direction)
    {
    case Direction::UP:
        drillPos.y -= size.y / 2 + 5;
        DrawRectangle(static_cast<int>(drillPos.x - 2), static_cast<int>(drillPos.y), 4, 8, YELLOW);
        break;
    case Direction::DOWN:
        drillPos.y += size.y / 2 + 5;
        DrawRectangle(static_cast<int>(drillPos.x - 2), static_cast<int>(drillPos.y - 8), 4, 8, YELLOW);
        break;
    case Direction::LEFT:
        drillPos.x -= size.x / 2 + 5;
        DrawRectangle(static_cast<int>(drillPos.x), static_cast<int>(drillPos.y - 2), 8, 4, YELLOW);
        break;
    case Direction::RIGHT:
        drillPos.x += size.x / 2 + 5;
        DrawRectangle(static_cast<int>(drillPos.x - 8), static_cast<int>(drillPos.y - 2), 8, 4, YELLOW);
        break;
    default:
        break;
    }

    // Draw direction indicator (small arrow)
    drawDirectionIndicator(center, direction, 4, WHITE);
}

void Sprite::drawRedMonster(Vector2 position, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw main body
    DrawCircleV(center, size.x / 2 - 2, RED);

    // Draw spikes/protrusions
    Vector2 spike1 = {center.x - 8, center.y - 8};
    Vector2 spike2 = {center.x + 8, center.y - 8};
    Vector2 spike3 = {center.x, center.y + 8};

    Color darkRed = {139, 0, 0, 255}; // Define dark red color

    DrawTriangle(spike1, {center.x - 4, center.y - 12}, {center.x - 12, center.y - 4}, darkRed);
    DrawTriangle(spike2, {center.x + 12, center.y - 4}, {center.x + 4, center.y - 12}, darkRed);
    DrawTriangle(spike3, {center.x - 4, center.y + 12}, {center.x + 4, center.y + 12}, darkRed);

    // Draw angry eyes
    DrawCircleV({center.x - 6, center.y - 2}, 3, WHITE);
    DrawCircleV({center.x + 6, center.y - 2}, 3, WHITE);
    DrawCircleV({center.x - 6, center.y - 2}, 1, RED);
    DrawCircleV({center.x + 6, center.y - 2}, 1, RED);

    // Draw mouth
    DrawRectangle(static_cast<int>(center.x - 4), static_cast<int>(center.y + 2), 8, 2, BLACK);
}

void Sprite::drawGreenDragon(Vector2 position, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw main body
    DrawCircleV(center, size.x / 2 - 2, DARKGREEN);

    // Draw dragon scales/texture
    DrawCircleV({center.x - 4, center.y - 4}, 2, GREEN);
    DrawCircleV({center.x + 4, center.y - 4}, 2, GREEN);
    DrawCircleV({center.x, center.y + 4}, 2, GREEN);

    // Draw dragon spikes (more elaborate than red monster)
    Vector2 topSpike = {center.x, center.y - 12};
    DrawTriangle(
        {center.x - 6, center.y - 6},
        topSpike,
        {center.x + 6, center.y - 6},
        LIME);

    // Draw dragon eyes (yellow for dragons)
    DrawCircleV({center.x - 6, center.y - 2}, 3, YELLOW);
    DrawCircleV({center.x + 6, center.y - 2}, 3, YELLOW);
    DrawCircleV({center.x - 6, center.y - 2}, 1, BLACK);
    DrawCircleV({center.x + 6, center.y - 2}, 1, BLACK);

    // Draw nostrils/snout
    DrawCircleV({center.x - 2, center.y + 2}, 1, BLACK);
    DrawCircleV({center.x + 2, center.y + 2}, 1, BLACK);
}

void Sprite::drawDisembodiedEyes(Vector2 position, Vector2 size, bool isGreen)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw floating eyes with ethereal effect
    Color eyeColor = isGreen ? YELLOW : WHITE;
    Color pupilColor = isGreen ? DARKGREEN : RED;

    // Draw outer glow
    DrawCircleV({center.x - 8, center.y}, 6, Fade(eyeColor, 0.3f));
    DrawCircleV({center.x + 8, center.y}, 6, Fade(eyeColor, 0.3f));

    // Draw main eyes
    DrawCircleV({center.x - 8, center.y}, 4, eyeColor);
    DrawCircleV({center.x + 8, center.y}, 4, eyeColor);

    // Draw pupils
    DrawCircleV({center.x - 8, center.y}, 2, pupilColor);
    DrawCircleV({center.x + 8, center.y}, 2, pupilColor);
}

void Sprite::drawHarpoon(Vector2 position, Direction direction, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    switch (direction)
    {
    case Direction::UP:
    case Direction::DOWN:
        // Vertical harpoon
        DrawRectangle(static_cast<int>(center.x - 1), static_cast<int>(position.y),
                      2, static_cast<int>(size.y), YELLOW);
        // Draw harpoon tip
        if (direction == Direction::UP)
        {
            DrawTriangle(
                {center.x - 3, position.y + 3},
                {center.x, position.y - 3},
                {center.x + 3, position.y + 3},
                ORANGE);
        }
        else
        {
            DrawTriangle(
                {center.x - 3, position.y + size.y - 3},
                {center.x, position.y + size.y + 3},
                {center.x + 3, position.y + size.y - 3},
                ORANGE);
        }
        break;

    case Direction::LEFT:
    case Direction::RIGHT:
        // Horizontal harpoon
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(center.y - 1),
                      static_cast<int>(size.x), 2, YELLOW);
        // Draw harpoon tip
        if (direction == Direction::LEFT)
        {
            DrawTriangle(
                {position.x + 3, center.y - 3},
                {position.x - 3, center.y},
                {position.x + 3, center.y + 3},
                ORANGE);
        }
        else
        {
            DrawTriangle(
                {position.x + size.x - 3, center.y - 3},
                {position.x + size.x + 3, center.y},
                {position.x + size.x - 3, center.y + 3},
                ORANGE);
        }
        break;

    default:
        break;
    }
}

void Sprite::drawCharacterSprite(Vector2 position, Vector2 size, Color bodyColor, Color accentColor, Direction direction)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw main body
    DrawCircleV(center, size.x / 2 - 2, bodyColor);

    // Draw accent details
    DrawCircleV({center.x, center.y - 4}, size.x / 4, accentColor);

    // Draw eyes
    drawEyes(center, 2.0f);

    // Draw direction indicator
    drawDirectionIndicator(center, direction, 3, WHITE);
}

void Sprite::drawRock(Vector2 position, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};
    float radius = size.x / 2 - 2;

    // Define rock colors
    Color baseRock = {120, 120, 120, 255};  // Medium gray
    Color darkRock = {80, 80, 80, 255};     // Dark gray
    Color lightRock = {160, 160, 160, 255}; // Light gray
    Color veryDarkRock = {50, 50, 50, 255}; // Very dark gray

    // Draw main rock body as an irregular shape using multiple circles
    DrawCircleV({center.x - 2, center.y - 2}, radius, baseRock);
    DrawCircleV({center.x + 1, center.y + 1}, radius - 3, darkRock);
    DrawCircleV({center.x - 1, center.y + 2}, radius - 5, baseRock);

    // Add rock texture with smaller circles
    DrawCircleV({center.x - 6, center.y - 4}, 3, lightRock);
    DrawCircleV({center.x + 4, center.y - 6}, 2, darkRock);
    DrawCircleV({center.x + 2, center.y + 4}, 2, veryDarkRock);
    DrawCircleV({center.x - 4, center.y + 6}, 2, lightRock);

    // Add some angular edges to make it look more rock-like
    Vector2 topLeft = {position.x + 2, position.y + 2};
    Vector2 topRight = {position.x + size.x - 2, position.y + 4};
    Vector2 bottomLeft = {position.x + 4, position.y + size.y - 2};
    Vector2 bottomRight = {position.x + size.x - 4, position.y + size.y - 2};

    // Draw rock edges/facets
    DrawTriangle(topLeft, {center.x - 8, center.y - 2}, {center.x - 2, center.y - 8}, darkRock);
    DrawTriangle(topRight, {center.x + 2, center.y - 8}, {center.x + 8, center.y - 2}, lightRock);
    DrawTriangle(bottomLeft, {center.x - 8, center.y + 2}, {center.x - 2, center.y + 8}, veryDarkRock);
    DrawTriangle(bottomRight, {center.x + 8, center.y + 2}, {center.x + 2, center.y + 8}, baseRock);

    // Add some small highlight spots to make it look more 3D
    DrawCircleV({center.x - 3, center.y - 3}, 1, WHITE);
    DrawCircleV({center.x + 5, center.y - 1}, 1, Color{200, 200, 200, 255});

    // Add a subtle outline
    DrawCircleLinesV(center, radius + 1, veryDarkRock);
}

void Sprite::drawEyes(Vector2 center, float eyeSize, Color eyeColor)
{
    float eyeOffset = 6.0f;

    // Draw eye whites
    DrawCircleV({center.x - eyeOffset, center.y - 2}, eyeSize + 1, eyeColor);
    DrawCircleV({center.x + eyeOffset, center.y - 2}, eyeSize + 1, eyeColor);

    // Draw pupils
    DrawCircleV({center.x - eyeOffset, center.y - 2}, eyeSize - 1, BLACK);
    DrawCircleV({center.x + eyeOffset, center.y - 2}, eyeSize - 1, BLACK);
}

void Sprite::drawDirectionIndicator(Vector2 center, Direction direction, float indicatorSize, Color color)
{
    Vector2 indicatorPos = center;

    switch (direction)
    {
    case Direction::UP:
        indicatorPos.y -= indicatorSize * 2;
        DrawTriangle(
            {indicatorPos.x - indicatorSize, indicatorPos.y + indicatorSize},
            {indicatorPos.x, indicatorPos.y - indicatorSize},
            {indicatorPos.x + indicatorSize, indicatorPos.y + indicatorSize},
            color);
        break;
    case Direction::DOWN:
        indicatorPos.y += indicatorSize * 2;
        DrawTriangle(
            {indicatorPos.x - indicatorSize, indicatorPos.y - indicatorSize},
            {indicatorPos.x, indicatorPos.y + indicatorSize},
            {indicatorPos.x + indicatorSize, indicatorPos.y - indicatorSize},
            color);
        break;
    case Direction::LEFT:
        indicatorPos.x -= indicatorSize * 2;
        DrawTriangle(
            {indicatorPos.x + indicatorSize, indicatorPos.y - indicatorSize},
            {indicatorPos.x - indicatorSize, indicatorPos.y},
            {indicatorPos.x + indicatorSize, indicatorPos.y + indicatorSize},
            color);
        break;
    case Direction::RIGHT:
        indicatorPos.x += indicatorSize * 2;
        DrawTriangle(
            {indicatorPos.x - indicatorSize, indicatorPos.y - indicatorSize},
            {indicatorPos.x + indicatorSize, indicatorPos.y},
            {indicatorPos.x - indicatorSize, indicatorPos.y + indicatorSize},
            color);
        break;
    default:
        DrawCircleV(indicatorPos, indicatorSize, color);
        break;
    }
}
