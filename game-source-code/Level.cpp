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
    // Create a horizontal tunnel near the top for the player to start in
    int startY = 2;
    for (int x = 10; x < 18; x++)
    {
        grid.digTunnel(x, startY);
    }

    // Create some vertical tunnels
    for (int y = 5; y < 10; y++)
    {
        grid.digTunnel(5, y);
        grid.digTunnel(14, y);
        grid.digTunnel(22, y);
    }

    // Create some horizontal connecting tunnels
    for (int x = 5; x < 15; x++)
    {
        grid.digTunnel(x, 8);
    }
    for (int x = 14; x < 23; x++)
    {
        grid.digTunnel(x, 12);
    }

    // Create bottom area tunnels
    for (int x = 8; x < 20; x++)
    {
        grid.digTunnel(x, 18);
    }
}

void Level::placeRocks()
{
    // Place some rocks in strategic positions
    std::vector<Vector2> rockGridPositions = {
        {7, 7}, {16, 7}, {12, 11}, {18, 15}, {6, 15}};

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