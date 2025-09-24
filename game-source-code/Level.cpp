#include "Level.h"

Level::Level()
{
    initializeDefault();
}

void Level::initializeDefault()
{
    // Clear any existing data
    monsterSpawnPositions.clear();
    rockPositions.clear();

    // Reset the grid completely (this will fill everything with earth again)
    grid = Grid(); // Create a fresh grid

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
    // Keep rocks minimal and strategic for first level
    // Later levels could have more rocks
    std::vector<Vector2> rockGridPositions = {
        {12, 6},  // Above center area
        {21, 9},  // Right side
        {13, 17}, // Above bottom tunnel
        {6, 10},  // Left side
        {16, 14}  // Mid-right
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

    // Level 1: Start with just 2-3 monsters in predictable locations
    // Add one monster to the right tunnel (x = 19-26, y = 15)
    Vector2 rightTunnelSpawn = grid.gridToWorld(22, 15);
    monsterSpawnPositions.push_back(rightTunnelSpawn);

    // Add one monster to the bottom tunnel (x = 8-20, y = 19)
    Vector2 bottomTunnelSpawn = grid.gridToWorld(10, 19);
    monsterSpawnPositions.push_back(bottomTunnelSpawn);

    // Add one monster to the left tunnel (x = 2-7, y = 10)
    Vector2 leftTunnelSpawn = grid.gridToWorld(4, 10);
    monsterSpawnPositions.push_back(leftTunnelSpawn);

    // That's it - just 3 monsters for a clean, manageable first level
    // Additional levels can add more monsters
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

// Add new method for level progression
void Level::initializeLevel(int levelNumber)
{
    // Clear existing data
    monsterSpawnPositions.clear();
    rockPositions.clear();

    // Create fresh grid
    grid = Grid();

    // Player always starts in the same position
    playerStartPosition = grid.gridToWorld(14, 2);

    // Create the same tunnel layout for consistency
    createInitialTunnels();

    // Vary rocks and monsters based on level
    placeLevelRocks(levelNumber);
    setLevelMonsterSpawns(levelNumber);
}

void Level::placeLevelRocks(int levelNumber)
{
    // Base rock positions
    std::vector<Vector2> baseRocks = {
        {12, 6},  // Above center area
        {21, 9},  // Right side
        {13, 17}, // Above bottom tunnel
    };

    // Add more rocks for higher levels
    if (levelNumber >= 2)
    {
        baseRocks.push_back({6, 10});  // Left side
        baseRocks.push_back({16, 14}); // Mid-right
    }

    if (levelNumber >= 3)
    {
        baseRocks.push_back({25, 13}); // Far right
        baseRocks.push_back({2, 12});  // Far left
    }

    for (const auto &gridPos : baseRocks)
    {
        int x = static_cast<int>(gridPos.x);
        int y = static_cast<int>(gridPos.y);

        grid.setTile(x, y, TileType::ROCK);
        rockPositions.push_back(grid.gridToWorld(x, y));
    }
}

void Level::setLevelMonsterSpawns(int levelNumber)
{
    monsterSpawnPositions.clear();

    // Base monster positions (always present)
    std::vector<Vector2> baseSpawns = {
        grid.gridToWorld(22, 15), // Right tunnel
        grid.gridToWorld(10, 19), // Bottom tunnel
        grid.gridToWorld(4, 10),  // Left tunnel
    };

    // Add base monsters
    for (const auto &spawn : baseSpawns)
    {
        monsterSpawnPositions.push_back(spawn);
    }

    // Add more monsters for higher levels
    if (levelNumber >= 2)
    {
        monsterSpawnPositions.push_back(grid.gridToWorld(15, 5));  // Center vertical
        monsterSpawnPositions.push_back(grid.gridToWorld(24, 15)); // Far right
    }

    if (levelNumber >= 3)
    {
        monsterSpawnPositions.push_back(grid.gridToWorld(6, 10));  // Left area
        monsterSpawnPositions.push_back(grid.gridToWorld(18, 19)); // Bottom right
    }

    // Cap at reasonable number even for very high levels
    if (levelNumber >= 4)
    {
        int maxMonsters = std::min(8, 3 + levelNumber);
        if (monsterSpawnPositions.size() > static_cast<size_t>(maxMonsters))
        {
            monsterSpawnPositions.resize(maxMonsters);
        }
    }
}

void Level::drawRock(Vector2 position, Vector2 size)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw main rock body
    DrawCircleV(center, size.x / 2 - 2, GRAY);

    // Draw some texture/details on the rock
    DrawCircleV({center.x - 4, center.y - 4}, 2, DARKGRAY);
    DrawCircleV({center.x + 4, center.y - 4}, 2, DARKGRAY);
    DrawCircleV({center.x, center.y + 4}, 2, DARKGRAY);
}

void Level::drawCharacterSprite(Vector2 position, Vector2 size, Color bodyColor, Color accentColor, Direction direction)
{
    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    // Draw main body
    DrawCircleV(center, size.x / 2 - 2, bodyColor);

    // Draw accent details
    DrawCircleV({center.x, center.y - 4}, size.x / 4, accentColor);

    // Draw eyes
    DrawCircleV({center.x - 6, center.y - 2}, 3, WHITE);
    DrawCircleV({center.x + 6, center.y - 2}, 3, WHITE);
    DrawCircleV({center.x - 6, center.y - 2}, 1, BLACK); // Left pupil
    DrawCircleV({center.x + 6, center.y - 2}, 1, BLACK); // Right pupil

    // Draw mouth
    DrawRectangle(static_cast<int>(center.x - 4), static_cast<int>(center.y + 4), 8, 2, BLACK);

    // Draw drill (based on direction)
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
}