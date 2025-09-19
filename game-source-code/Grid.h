#ifndef GRID_H
#define GRID_H

#include <vector>
#include <raylib-cpp.hpp>
#include "GameEnums.h"

/**
 * @brief Manages the underground grid system for the game
 */
class Grid
{
public:
    /**
     * @brief Constructor for Grid
     * @param gridWidth Width of the grid in tiles
     * @param gridHeight Height of the grid in tiles
     * @param tileSize Size of each tile in pixels
     */
    Grid(int gridWidth = 28, int gridHeight = 22, int tileSize = 32);

    /**
     * @brief Get the tile type at a specific grid position
     * @param x Grid x coordinate
     * @param y Grid y coordinate
     * @return TileType at that position
     */
    TileType getTile(int x, int y) const;

    /**
     * @brief Set the tile type at a specific grid position
     * @param x Grid x coordinate
     * @param y Grid y coordinate
     * @param type New tile type
     */
    void setTile(int x, int y, TileType type);

    /**
     * @brief Check if a position is a tunnel
     * @param x Grid x coordinate
     * @param y Grid y coordinate
     * @return true if the position is a tunnel
     */
    bool isTunnel(int x, int y) const;

    /**
     * @brief Dig a tunnel at the specified position
     * @param x Grid x coordinate
     * @param y Grid y coordinate
     */
    void digTunnel(int x, int y);

    /**
     * @brief Check if grid coordinates are valid
     * @param x Grid x coordinate
     * @param y Grid y coordinate
     * @return true if coordinates are within bounds
     */
    bool isValidPosition(int x, int y) const;

    /**
     * @brief Convert world position to grid coordinates
     * @param worldPos World position in pixels
     * @return Grid coordinates as Vector2
     */
    Vector2 worldToGrid(Vector2 worldPos) const;

    Vector2 gridToWorld(int gridX, int gridY) const;

    int getWidth() const;

    int getHeight() const;
    int getTileSize() const;
    void drawGrid() const;
    void drawTiles() const;

private:
    int width;                                // Grid width in tiles
    int height;                               // Grid height in tiles
    int tileSize;                             // Size of each tile in pixels
    std::vector<std::vector<TileType>> tiles; // 2D grid of tiles
    void initializeGrid();
};

#endif // GRID_H