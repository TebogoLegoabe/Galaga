#include "Level.h"

Level::Level()
{
    initializeDefault();
}

void Level::initializeDefault()
{
    // Set player start position (center top area)
    playerStartPosition = grid.gridToWorld(14, 2);

    createInitialTunnels();
    placeRocks();
    setMonsterSpawns();
}

bool Level::loadFromFile(const std::string &filename)
{
    // TODO: Implement file loading in future versions
    // For now, just use default initialization
    initializeDefault();
    return true;
}

Grid &Level::getGrid()
{
    return grid;
}

const Grid &Level::getGrid() const
{
    return grid;
}

Vector2 Level::getPlayerStartPosition() const
{
    return playerStartPosition;
}

std::vector<Vector2> Level::getMonsterSpawnPositions() const
{
    return monsterSpawnPositions;
}

std::vector<Vector2> Level::getRockPositions() const
{
    return rockPositions;
}

void Level::draw() const
{
    grid.drawTiles();

    // Optionally draw grid lines for debugging
    // grid.drawGrid();
}

bool Level::isWithinBounds(Vector2 worldPos) const
{
    return worldPos.x >= 0 &&
           worldPos.y >= 0 &&
           worldPos.x < grid.getWidth() * grid.getTileSize() &&
           worldPos.y < grid.getHeight() * grid.getTileSize();
}

void Level::createInitialTunnels()
{
    // Create horizontal tunnel at top where player starts
    int topTunnelY = 2;
    for (int x = 12; x < 18; x++)
    {
        grid.digTunnel(x, topTunnelY);
    }

    // Create vertical tunnels connecting different levels
    // Left vertical tunnel
    for (int y = 2; y < 18; y++)
    {
        grid.digTunnel(15, y);
    }

    // Center-left vertical tunnel
    for (int y = 3; y < 20; y++)
    {
        // grid.digTunnel(8, y);
    }

    // Center-right vertical tunnel
    for (int y = 8; y < 18; y++)
    {
        // grid.digTunnel(19, y);
    }

    // Right vertical tunnel
    for (int y = 12; y < 19; y++)
    {
        // grid.digTunnel(24, y);
    }

    // Create horizontal tunnels at different depths
    // Second level horizontal tunnel (left)
    for (int x = 3; x < 9; x++)
    {
        // grid.digTunnel(x, 8);
    }

    // Second level horizontal tunnel (right)
    for (int x = 15; x < 22; x++)
    {
        // grid.digTunnel(x, 11);
    }

    // Third level horizontal tunnel (left)
    for (int x = 1; x < 9; x++)
    {
        // grid.digTunnel(x, 15);
    }

    // Third level horizontal tunnel (right)
    for (int x = 19; x < 26; x++)
    {
        grid.digTunnel(x, 15);
    }

    // Bottom level horizontal tunnel
    for (int x = 8; x < 20; x++)
    {
        grid.digTunnel(x, 19);
    }
}

void Level::placeRocks()
{
    // Place rocks in strategic positions that can fall and crush monsters
    std::vector<Vector2> rockGridPositions = {
        /*{12, 6},  // Above center area
        {6, 10},  // Left side
        {21, 9},  // Right side
        {10, 13}, // Mid-left
        {16, 14}, // Mid-right
        {13, 17}, // Above bottom tunnel
        {25, 13}, // Far right
        {2, 12}   // Far left */
    };

    for (const auto &gridPos : rockGridPositions)
    {
        int x = static_cast<int>(gridPos.x);
        int y = static_cast<int>(gridPos.y);

        grid.setTile(x, y, TileType::ROCK);
        rockPositions.push_back(grid.gridToWorld(x, y));
    }
}

void Level::setMonsterSpawns()
{
    // Place monsters in tunnels, away from player start
    std::vector<Vector2> spawnGridPositions = {
        {5, 8},   // Left tunnel
        {22, 8},  // Right tunnel
        {14, 12}, // Middle tunnel
        {10, 18}  // Bottom tunnel
    };

    for (const auto &gridPos : spawnGridPositions)
    {
        monsterSpawnPositions.push_back(
            grid.gridToWorld(static_cast<int>(gridPos.x),
                             static_cast<int>(gridPos.y)));
    }
}