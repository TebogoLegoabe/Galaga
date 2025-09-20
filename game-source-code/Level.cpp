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
    // Clear any existing spawns
    monsterSpawnPositions.clear();

    // Find all tunnel positions that are suitable for monster spawns
    std::vector<Vector2> potentialSpawns;

    for (int y = 0; y < grid.getHeight(); y++)
    {
        for (int x = 0; x < grid.getWidth(); x++)
        {
            if (grid.getTile(x, y) == TileType::TUNNEL)
            {
                Vector2 worldPos = grid.gridToWorld(x, y);
                // Don't spawn too close to player start
                Vector2 playerGridPos = grid.worldToGrid(playerStartPosition);
                float distance = std::sqrt(std::pow(x - playerGridPos.x, 2) + std::pow(y - playerGridPos.y, 2));

                if (distance > 3.0f) // At least 3 tiles away from player
                {
                    potentialSpawns.push_back(worldPos);
                }
            }
        }
    }

    // Select a few well-distributed spawn points
    if (!potentialSpawns.empty())
    {
        // Add monsters to different areas of the map
        std::vector<Vector2> selectedSpawns;

        // Try to get spawns from different quadrants
        for (const auto &spawn : potentialSpawns)
        {
            Vector2 gridPos = grid.worldToGrid(spawn);
            bool tooClose = false;

            // Check if this spawn is too close to existing spawns
            for (const auto &existing : selectedSpawns)
            {
                Vector2 existingGrid = grid.worldToGrid(existing);
                float distance = std::sqrt(std::pow(gridPos.x - existingGrid.x, 2) +
                                           std::pow(gridPos.y - existingGrid.y, 2));
                if (distance < 4.0f) // Keep monsters at least 4 tiles apart
                {
                    tooClose = true;
                    break;
                }
            }

            if (!tooClose)
            {
                selectedSpawns.push_back(spawn);
                if (selectedSpawns.size() >= 4) // Limit to 4 monsters
                    break;
            }
        }

        monsterSpawnPositions = selectedSpawns;
    }
}

bool Level::isSuitableForMonsterSpawn(int gridX, int gridY, const std::vector<Vector2> &existingSpawns) const
{
    // Check if position is a tunnel
    if (grid.getTile(gridX, gridY) != TileType::TUNNEL)
        return false;

    // Check distance from player start
    Vector2 playerGridPos = grid.worldToGrid(playerStartPosition);
    float distanceFromPlayer = std::sqrt(std::pow(gridX - playerGridPos.x, 2) +
                                         std::pow(gridY - playerGridPos.y, 2));
    if (distanceFromPlayer < 3.0f)
        return false;

    // Check distance from existing spawns
    Vector2 worldPos = grid.gridToWorld(gridX, gridY);
    for (const auto &existing : existingSpawns)
    {
        Vector2 existingGrid = grid.worldToGrid(existing);
        float distance = std::sqrt(std::pow(gridX - existingGrid.x, 2) +
                                   std::pow(gridY - existingGrid.y, 2));
        if (distance < 4.0f)
            return false;
    }

    return true;
}