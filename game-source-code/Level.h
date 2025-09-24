#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <raylib-cpp.hpp>
#include "Grid.h"

/**
 * @brief Manages level data and initial setup
 */
class Level
{
public:
    /**
     * @brief Constructor for Level
     */
    Level();

    /**
     * @brief Initialize the level with default layout
     */
    void initializeDefault();

    /**
     * @brief Initialize a specific level number
     * @param levelNumber The level number (1, 2, 3, etc.)
     */
    void initializeLevel(int levelNumber);

    /**
     * @brief Load level from file (future implementation)
     * @param filename Path to the level file
     * @return true if successfully loaded
     */
    bool loadFromFile(const std::string &filename);

    /**
     * @brief Get the grid for this level
     * @return Reference to the grid
     */
    Grid &getGrid();

    /**
     * @brief Get the grid for this level (const version)
     * @return Const reference to the grid
     */
    const Grid &getGrid() const;

    /**
     * @brief Get the player start position
     * @return Player starting position in world coordinates
     */
    Vector2 getPlayerStartPosition() const;

    /**
     * @brief Get monster spawn positions
     * @return Vector of monster spawn positions in world coordinates
     */
    std::vector<Vector2> getMonsterSpawnPositions() const;

    /**
     * @brief Get rock positions
     * @return Vector of rock positions in world coordinates
     */
    std::vector<Vector2> getRockPositions() const;

    /**
     * @brief Draw the level
     */
    void draw() const;

    /**
     * @brief Check if position is within level bounds
     * @param worldPos Position in world coordinates
     * @return true if within bounds
     */
    bool isWithinBounds(Vector2 worldPos) const;

private:
    Grid grid;                                  ///< The level's grid
    Vector2 playerStartPosition;                ///< Player starting position
    std::vector<Vector2> monsterSpawnPositions; ///< Monster spawn positions
    std::vector<Vector2> rockPositions;         ///< Rock positions

    /**
     * @brief Create some initial tunnels for the level
     */
    void createInitialTunnels();

    /**
     * @brief Place rocks in the level
     */
    void placeRocks();

    /**
     * @brief Place rocks for a specific level
     * @param levelNumber Level number
     */
    void placeLevelRocks(int levelNumber);

    /**
     * @brief Set monster spawn positions (one per tunnel)
     */
    void setMonsterSpawns();

    /**
     * @brief Draw a rock
     * @param position Position to draw at
     * @param size Size of the rock
     */
    static void drawRock(Vector2 position, Vector2 size);

    /**
     * @brief Draw a simple character sprite with customizable colors
     * @param position Position to draw at
     * @param size Size of the sprite
     * @param bodyColor Main body color
     * @param accentColor Accent/detail color
     * @param direction Direction facing (for asymmetric sprites)
     */
    static void drawCharacterSprite(Vector2 position, Vector2 size, Color bodyColor, Color accentColor, Direction direction = Direction::RIGHT);

    /**
     * @brief Set monster spawns for a specific level
     * @param levelNumber Level number
     */
    void setLevelMonsterSpawns(int levelNumber);

    /**
     * @brief Check if a position is in a tunnel and not occupied
     * @param gridX Grid X coordinate
     * @param gridY Grid Y coordinate
     * @param existingSpawns Existing spawn positions to avoid
     * @return true if position is suitable for monster spawn
     */
    bool isSuitableForMonsterSpawn(int gridX, int gridY, const std::vector<Vector2> &existingSpawns) const;
};

#endif // LEVEL_H