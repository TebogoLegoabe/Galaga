# Galaga/Dig Dug Game Architecture

## Class Diagram

```mermaid
classDiagram
    %% Core Game Classes
    class Game {
        -bool isRunning
        -raylib::Window window
        -GameStateManager stateManager
        +Game()
        +~Game()
        +void init()
        +void run()
        +void update()
        +void draw()
        +void handleWindowEvents()
        +void cleanup()
    }

    class GameStateManager {
        -GameState currentState
        -std::unique_ptr~Menu~ menu
        -std::unique_ptr~GamePlay~ gamePlay
        +GameStateManager()
        +void init()
        +void update()
        +void draw()
        +void handleInput()
        +GameState getCurrentState()
        +void setState(GameState state)
        +void transitionToMenu()
        +void transitionToGamePlay()
    }

    %% Game States
    class Menu {
        -MenuState currentMenuState
        -bool startGame
        -bool exitGame
        +Menu()
        +void handleInput()
        +void update()
        +void draw()
        +MenuState getMenuState()
        +bool shouldStartGame()
        +bool shouldExitGame()
        +void reset()
        +void setGameOver(bool playerWon)
        +void setLevelComplete()
    }

    class GamePlay {
        -Level currentLevel
        -Player player
        -std::vector~std::unique_ptr~Monster~~ monsters
        -bool gameOver
        -bool levelComplete
        -bool playerWon
        -float disembodiedCooldown
        +GamePlay()
        +void init()
        +void handleInput()
        +void update()
        +void draw()
        +bool isGameOver()
        +bool isLevelComplete()
        +bool didPlayerWin()
        +void reset()
        -void updateGameLogic()
        -void drawHUD()
        -void handlePlayerMovement()
        -void initializeMonsters()
        -void updateMonsters()
        -void drawMonsters()
        -void checkPlayerMonsterCollisions()
        -void checkHarpoonMonsterCollisions()
    }

    %% Abstract Base Class
    class GameObject {
        <<abstract>>
        #Vector2 position
        #Vector2 size
        #bool active
        +GameObject(Vector2 pos, Vector2 objSize)
        +virtual ~GameObject()
        +virtual void update()*
        +virtual void draw()*
        +Rectangle getBounds()
        +bool isActive()
        +void setActive(bool active)
        +Vector2 getPosition()
        +void setPosition(Vector2 newPos)
        +Vector2 getSize()
        #bool isWithinBounds(int screenWidth, int screenHeight)
    }

    %% Game Entities
    class Player {
        -Direction facingDirection
        -float speed
        -std::unique_ptr~Harpoon~ harpoon
        -bool canMove
        -int lives
        +Player(Vector2 startPos)
        +void update()
        +void draw()
        +bool move(Direction direction, Grid& grid)
        +bool shoot()
        +Direction getFacingDirection()
        +bool canMoveTo(Vector2 newPos, Grid& grid)
        +Vector2 getGridPosition(Grid& grid)
        +void reset(Vector2 startPos)
        +float getSpeed()
        +void setSpeed(float newSpeed)
        +Harpoon& getHarpoon()
        +bool canShoot()
        +int getLives()
        +bool loseLife()
        -void updateMovement()
        -void updateShooting()
        -void digAtCurrentPosition(Grid& grid)
    }

    class Monster {
        #MonsterState currentState
        #float speed
        #Vector2 targetPosition
        #bool isMoving
        #float stateTimer
        #float aiUpdateTimer
        #Direction lastDirection
        +Monster(Vector2 startPos, MonsterState state)
        +void update()
        +void draw()
        +void updateAI(Player& player, Grid& grid, bool canBecomeDisembodied, std::function~void()~ notifyDisembodied)
        +bool move(Direction direction, Grid& grid)
        +MonsterState getState()
        +void setState(MonsterState newState)
        +Vector2 getGridPosition(Grid& grid)
        +bool canMoveTo(Vector2 newPos, Grid& grid)
        +void reset(Vector2 startPos, MonsterState state)
        +float getSpeed()
        +void setSpeed(float newSpeed)
        +bool isDead()
        #void updateMovement()
        #void updateStateTimer()
        -Direction findBestDirectionToPlayer(Player& player, Grid& grid)
        -Direction findRandomValidDirection(Grid& grid)
        -bool shouldBecomeDisembodied(Player& player, Grid& grid)
    }

    class RedMonster {
        +RedMonster(Vector2 startPos)
        +void update()
        +void draw()
        +void updateAI(Vector2 playerPos, Grid& grid)
        -Direction findBestDirectionToPlayer(Vector2 playerPos, Grid& grid)
        -Direction findRandomValidDirection(Grid& grid)
    }

    class Harpoon {
        -Direction direction
        -float speed
        -float maxRange
        -float travelDistance
        -bool fired
        +Harpoon(Vector2 startPos, Direction direction)
        +void update()
        +void draw()
        +void fire(Vector2 startPos, Direction dir)
        +Direction getDirection()
        +bool isHarpoonActive()
        +void deactivate()
        +bool shouldDestroy(Grid& grid)
        +void reset()
        +float getSpeed()
        +void setSpeed(float newSpeed)
        -void updateMovement()
        -bool isWithinBounds(Grid& grid)
    }

    class Rock {
        -RockState currentState
        -float speed
        -float fallDelay
        -float fallTimer
        -Vector2 originalPosition
        +Rock(Vector2 startPos)
        +void update()
        +void draw()
        +void checkShouldFall(Grid& grid)
        +RockState getState()
        +Vector2 getGridPosition(Grid& grid)
        +bool isFalling()
        +void reset(Vector2 startPos)
        +float getSpeed()
        +void setSpeed(float newSpeed)
        -void updateFalling(Grid& grid)
        -bool hasGroundBelow(Grid& grid)
    }

    %% Game Systems
    class Grid {
        -int width
        -int height
        -int tileSize
        -std::vector~std::vector~TileType~~ tiles
        +Grid(int gridWidth, int gridHeight, int tileSize)
        +TileType getTile(int x, int y)
        +void setTile(int x, int y, TileType type)
        +bool isTunnel(int x, int y)
        +void digTunnel(int x, int y)
        +bool isValidPosition(int x, int y)
        +Vector2 worldToGrid(Vector2 worldPos)
        +Vector2 gridToWorld(int gridX, int gridY)
        +int getWidth()
        +int getHeight()
        +int getTileSize()
        +void drawGrid()
        +void drawTiles()
        -void initializeGrid()
    }

    class Level {
        -Grid grid
        -Vector2 playerStartPosition
        -std::vector~Vector2~ monsterSpawnPositions
        -std::vector~Vector2~ rockPositions
        +Level()
        +void initializeDefault()
        +void initializeLevel(int levelNumber)
        +bool loadFromFile(string filename)
        +Grid& getGrid()
        +Vector2 getPlayerStartPosition()
        +std::vector~Vector2~ getMonsterSpawnPositions()
        +std::vector~Vector2~ getRockPositions()
        +void draw()
        +bool isWithinBounds(Vector2 worldPos)
        -void createInitialTunnels()
        -void placeRocks()
        -void setMonsterSpawns()
        -void placeLevelRocks(int levelNumber)
        -void setLevelMonsterSpawns(int levelNumber)
    }

    class CollisionManager {
        +static bool checkCollision(GameObject& obj1, GameObject& obj2)
        +static bool checkPlayerMonsterCollision(Player& player, std::vector~std::unique_ptr~Monster~~& monsters)
        +static bool checkHarpoonMonsterCollision(Harpoon& harpoon, std::vector~std::unique_ptr~Monster~~& monsters)
        +static bool checkRockPlayerCollision(std::vector~Rock~& rocks, Player& player)
    }

    class InputHandler {
        +static Direction getMovementInput()
        +static bool isShootPressed()
        +static bool isMenuPressed()
        +static bool isPausePressed()
    }

    class Sprite {
        -Vector2 position
        -Vector2 size
        +Sprite(Vector2 pos, Vector2 spriteSize)
        +static void drawDigDug(Vector2 position, Direction direction, Vector2 size)
        +static void drawRedMonster(Vector2 position, Vector2 size)
        +static void drawRock(Vector2 position, Vector2 size)
        +static void drawHarpoon(Vector2 position, Direction direction, Vector2 size)
        +static void drawTile(Vector2 position, TileType type, Vector2 size)
    }

    %% Enums
    class GameEnums {
        <<enumeration>>
        GameState
        Direction
        TileType
        MonsterState
        MenuState
        RockState
    }

    %% Inheritance Relationships
    GameObject <|-- Player
    GameObject <|-- Monster
    GameObject <|-- Harpoon
    GameObject <|-- Rock
    Monster <|-- RedMonster

    %% Composition Relationships
    Game *-- GameStateManager
    GameStateManager *-- Menu
    GameStateManager *-- GamePlay
    GamePlay *-- Level
    GamePlay *-- Player
    GamePlay *-- Monster
    Level *-- Grid
    Player *-- Harpoon

    %% Dependencies
    GamePlay ..> CollisionManager : uses
    GamePlay ..> InputHandler : uses
    Player ..> Grid : uses
    Monster ..> Grid : uses
    Level ..> Grid : manages
    Sprite ..> GameEnums : uses
    Player ..> Sprite : uses
    Monster ..> Sprite : uses
    Rock ..> Sprite : uses
    Harpoon ..> Sprite : uses
```

## System Flow Diagram

```mermaid
flowchart TD
    A[Game Start] --> B[Initialize Game]
    B --> C[GameStateManager]
    C --> D{Current State?}

    D -->|MENU| E[Menu System]
    D -->|PLAYING| F[GamePlay System]
    D -->|GAME_OVER| G[Game Over Screen]
    D -->|LEVEL_COMPLETE| H[Level Complete Screen]

    E --> E1[Handle Menu Input]
    E1 --> E2[Update Menu State]
    E2 --> E3[Draw Menu]
    E3 --> E4{Start Game?}
    E4 -->|Yes| F
    E4 -->|No| E1

    F --> F1[Initialize Level]
    F1 --> F2[Game Loop]
    F2 --> F3[Handle Input]
    F3 --> F4[Update Player]
    F4 --> F5[Update Monsters]
    F5 --> F6[Update Harpoons]
    F6 --> F7[Check Collisions]
    F7 --> F8[Update Grid]
    F8 --> F9[Draw Everything]
    F9 --> F10{Game Over?}

    F10 -->|No| F3
    F10 -->|Player Dies| G
    F10 -->|Level Complete| H

    G --> G1[Show Game Over]
    G1 --> G2{Return to Menu?}
    G2 -->|Yes| E
    G2 -->|No| I[Exit Game]

    H --> H1[Show Level Complete]
    H1 --> H2{Next Level?}
    H2 -->|Yes| F1
    H2 -->|Return to Menu| E

    I --> J[Game End]
```

## Component Interaction Diagram

```mermaid
sequenceDiagram
    participant G as Game
    participant GSM as GameStateManager
    participant GP as GamePlay
    participant P as Player
    participant M as Monster
    participant Gr as Grid
    participant CM as CollisionManager
    participant L as Level

    G->>GSM: update()
    GSM->>GP: update()

    GP->>P: handleInput()
    GP->>P: update()
    P->>Gr: move(direction, grid)
    P->>Gr: digTunnel(x, y)

    GP->>M: updateAI(player, grid)
    M->>Gr: canMoveTo(newPos, grid)
    M->>M: move(direction, grid)

    GP->>CM: checkPlayerMonsterCollisions()
    CM->>P: getBounds()
    CM->>M: getBounds()
    CM-->>GP: collision result

    GP->>CM: checkHarpoonMonsterCollisions()
    CM-->>GP: collision result

    GP->>L: draw()
    L->>Gr: drawTiles()
    GP->>P: draw()
    GP->>M: draw()
```

## Architecture Summary

### Core Components:

1. **Game** - Main game loop and window management
2. **GameStateManager** - Handles transitions between game states
3. **GamePlay** - Main gameplay logic and entity management
4. **GameObject** - Abstract base class for all game entities
5. **Grid** - Underground tunnel/earth system
6. **Level** - Level layout and initialization

### Key Design Patterns:

- **State Pattern**: GameStateManager manages different game states
- **Inheritance**: GameObject hierarchy for common functionality
- **Composition**: GamePlay contains Player, Monsters, Level
- **Static Utilities**: CollisionManager, InputHandler, Sprite rendering
- **Template/Strategy**: Monster AI with different behaviors

### Data Flow:

1. Input → GameStateManager → Current State (Menu/GamePlay)
2. GamePlay manages all entities and game logic
3. Grid system handles tunnel digging and collision detection
4. CollisionManager handles all collision detection
5. Sprite system handles all rendering

This architecture follows good object-oriented principles with clear separation of concerns, inheritance hierarchies, and modular design.
