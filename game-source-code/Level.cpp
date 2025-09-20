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

    // Third level horizontal tunnel (right)
    for (int x = 19; x < 26; x++)
    {
        grid.digTunnel(x, 15);
    }

    // Third level horizontal tunnel (left)
    for (int x = 2; x < 7; x++)
    {
        grid.digTunnel(x, 10);
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
    // Clear any existing spawns
    monsterSpawnPositions.clear();

    // Add one monster to the left tunnel (x = 2-7, y = 10)
    Vector2 leftTunnelSpawn = grid.gridToWorld(2, 10);
    // monsterSpawnPositions.push_back(leftTunnelSpawn);

    // Add one monster to the right tunnel (x = 19-26, y = 15)
    Vector2 rightTunnelSpawn = grid.gridToWorld(22, 15);
    monsterSpawnPositions.push_back(rightTunnelSpawn);

    // Additional spawns can be added here as needed
    Vector2 thirdTunnelSpawn = grid.gridToWorld(15, 5);
    monsterSpawnPositions.push_back(thirdTunnelSpawn);
    Vector2 bottomTunnelSpawn = grid.gridToWorld(10, 19);
    monsterSpawnPositions.push_back(bottomTunnelSpawn);
}

bool Level::isSuitableForMonsterSpawn(int gridX, int gridY, const std::vector<Vector2> &existingSpawns) const
{
    // Check if position is a tunnel
    if (grid.getTile(gridX, gridY) != TileType::TUNNEL)
        return false;

    // Check distance from player start (should be at least 3 tiles away)
    Vector2 playerGridPos = grid.worldToGrid(playerStartPosition);
    float distanceFromPlayer = std::sqrt(std::pow(gridX - playerGridPos.x, 2) +
                                         std::pow(gridY - playerGridPos.y, 2));
    if (distanceFromPlayer < 3.0f)
        return false;

    // Check distance from existing spawns (should be at least 2 tiles apart)
    Vector2 worldPos = grid.gridToWorld(gridX, gridY);
    for (const auto &existing : existingSpawns)
    {
        Vector2 existingGrid = grid.worldToGrid(existing);
        float distance = std::sqrt(std::pow(gridX - existingGrid.x, 2) +
                                   std::pow(gridY - existingGrid.y, 2));
        if (distance < 2.0f)
            return false;
    }

    return true;
}
