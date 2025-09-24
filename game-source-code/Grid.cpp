#include "Grid.h"
#include "Sprite.h"
#include <raylib-cpp.hpp>
#include "Level.h"

Grid::Grid(int gridWidth, int gridHeight, int tileSize)
    : width(gridWidth), height(gridHeight), tileSize(tileSize)
{
    initializeGrid();
}

TileType Grid::getTile(int x, int y) const
{
    if (!isValidPosition(x, y))
    {
        return TileType::EARTH; // Default to earth for invalid positions
    }
    return tiles[y][x];
}

void Grid::setTile(int x, int y, TileType type)
{
    if (isValidPosition(x, y))
    {
        tiles[y][x] = type;
    }
}

bool Grid::isTunnel(int x, int y) const
{
    return getTile(x, y) == TileType::TUNNEL;
}

void Grid::digTunnel(int x, int y)
{
    if (isValidPosition(x, y) && getTile(x, y) == TileType::EARTH)
    {
        setTile(x, y, TileType::TUNNEL);
    }
}

bool Grid::isValidPosition(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

Vector2 Grid::worldToGrid(Vector2 worldPos) const
{
    return Vector2{
        static_cast<float>(static_cast<int>(worldPos.x) / tileSize),
        static_cast<float>(static_cast<int>(worldPos.y) / tileSize)};
}

Vector2 Grid::gridToWorld(int gridX, int gridY) const
{
    return Vector2{
        static_cast<float>(gridX * tileSize),
        static_cast<float>(gridY * tileSize)};
}

int Grid::getWidth() const
{
    return width;
}

int Grid::getHeight() const
{
    return height;
}

int Grid::getTileSize() const
{
    return tileSize;
}

void Grid::drawGrid() const
{
    // Draw grid lines for debugging
    for (int x = 0; x <= width; x++)
    {
        DrawLine(x * tileSize, 0, x * tileSize, height * tileSize, GRAY);
    }
    for (int y = 0; y <= height; y++)
    {
        DrawLine(0, y * tileSize, width * tileSize, y * tileSize, GRAY);
    }
}

void Grid::drawTiles() const
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Vector2 worldPos = gridToWorld(x, y);
            Rectangle tileRect = {worldPos.x, worldPos.y,
                                  static_cast<float>(tileSize),
                                  static_cast<float>(tileSize)};
            Vector2 tileSize2D = {static_cast<float>(tileSize), static_cast<float>(tileSize)};

            switch (getTile(x, y))
            {
            case TileType::EARTH:
                DrawRectangleRec(tileRect, DARKBROWN);
                break;
            case TileType::TUNNEL:
                DrawRectangleRec(tileRect, BLACK);
                break;
            case TileType::ROCK:
                // Use the Sprite class to draw a nice rock instead of a gray rectangle
                Sprite::drawRock(worldPos, tileSize2D);
                break;
            }
        }
    }
}

void Grid::initializeGrid()
{
    // Initialize the 2D vector with all earth tiles
    tiles.resize(height);
    for (int y = 0; y < height; y++)
    {
        tiles[y].resize(width, TileType::EARTH);
    }
}