#include "RedMonster.h"

RedMonster::RedMonster(Vector2 startPos)
    : Monster(startPos, MonsterState::IN_TUNNEL)
{
    // Red monsters are slightly faster than base monsters
    setSpeed(1.8f);
}

void RedMonster::update()
{
    // Call base class update first
    Monster::update();

    // Add any red monster specific update logic here
}

void RedMonster::draw()
{
    if (!active || isDead())
        return;

    Vector2 center = {position.x + size.x / 2, position.y + size.y / 2};

    switch (currentState)
    {
    case MonsterState::IN_TUNNEL:
    {
        // Draw as red monster with more details
        DrawCircleV(center, size.x / 2 - 2, RED);

        // Draw spikes/details to make it look more menacing
        Color darkRed = {139, 0, 0, 255}; // Define DARKRED manually
        DrawCircleV({center.x - 8, center.y - 6}, 2, darkRed);
        DrawCircleV({center.x + 8, center.y - 6}, 2, darkRed);
        DrawCircleV({center.x, center.y - 10}, 2, darkRed);

        // Draw angry eyes
        DrawCircleV({center.x - 6, center.y - 4}, 2, WHITE);
        DrawCircleV({center.x + 6, center.y - 4}, 2, WHITE);
        DrawCircleV({center.x - 6, center.y - 4}, 1, BLACK); // Left pupil
        DrawCircleV({center.x + 6, center.y - 4}, 1, BLACK); // Right pupil

        // Draw angry mouth
        DrawRectangle(center.x - 4, center.y + 2, 8, 3, BLACK);
        break;
    }

    case MonsterState::DISEMBODIED:
        // Draw as red floating eyes
        DrawCircleV({center.x - 8, center.y}, 6, PINK);
        DrawCircleV({center.x + 8, center.y}, 6, PINK);
        DrawCircleV({center.x - 8, center.y}, 3, RED); // Left pupil
        DrawCircleV({center.x + 8, center.y}, 3, RED); // Right pupil
        break;

    case MonsterState::DEAD:
        // Don't draw dead monsters
        break;
    }
}

void RedMonster::updateAI(Vector2 playerPos, Grid &grid)
{
    if (!active || isDead())
        return;

    // Red monsters are MORE aggressive and strategic
    aiUpdateTimer += GetFrameTime();

    // Update AI every 0.2 seconds (faster than base monsters)
    if (aiUpdateTimer < 0.2f)
        return;

    aiUpdateTimer = 0.0f;

    // Calculate distance to player
    Vector2 playerGridPos = grid.worldToGrid(playerPos);
    Vector2 monsterGridPos = grid.worldToGrid(position);
    float distance = std::abs(playerGridPos.x - monsterGridPos.x) + std::abs(playerGridPos.y - monsterGridPos.y);

    // Strategic behavior based on distance and state
    if (currentState == MonsterState::IN_TUNNEL)
    {
        // Red monsters are more aggressive - less likely to wait
        if (distance <= 2.0f) // Very close - attack immediately
        {
            if (!isMoving)
            {
                Direction moveDirection = findBestDirectionToPlayer(playerPos, grid);
                if (moveDirection != Direction::NONE)
                {
                    move(moveDirection, grid);
                }
            }
        }
        else if (distance <= 4.0f) // Medium distance - move frequently
        {
            if (!isMoving && (rand() % 2 == 0)) // 50% chance to move (more than base monsters)
            {
                Direction moveDirection = findBestDirectionToPlayer(playerPos, grid);
                if (moveDirection != Direction::NONE)
                {
                    move(moveDirection, grid);
                }
            }
        }
        else if (distance > 6.0f && stateTimer > 2.5f) // Far away - become disembodied sooner
        {
            setState(MonsterState::DISEMBODIED);
            stateTimer = 0.0f;
        }
        else if (!isMoving && (rand() % 4 == 0)) // 25% chance for distant movement
        {
            Direction moveDirection = findRandomValidDirection(grid);
            if (moveDirection != Direction::NONE)
            {
                move(moveDirection, grid);
            }
        }
    }
    else if (currentState == MonsterState::DISEMBODIED)
    {
        // Disembodied red monsters are very aggressive
        if (stateTimer > 3.5f) // Return to tunnel slightly sooner
        {
            Vector2 gridPos = grid.worldToGrid(position);
            if (grid.isTunnel(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y)))
            {
                setState(MonsterState::IN_TUNNEL);
                stateTimer = 0.0f;
            }
        }

        // Always try to move towards player when disembodied
        if (!isMoving)
        {
            Direction moveDirection = findBestDirectionToPlayer(playerPos, grid);
            if (moveDirection != Direction::NONE)
            {
                move(moveDirection, grid);
            }
        }
    }

    // Update state timer
    stateTimer += GetFrameTime();
}

Direction RedMonster::findBestDirectionToPlayer(Vector2 playerPos, const Grid &grid)
{
    Vector2 monsterGridPos = grid.worldToGrid(position);
    Vector2 playerGridPos = grid.worldToGrid(playerPos);

    // Calculate direction to player
    float dx = playerGridPos.x - monsterGridPos.x;
    float dy = playerGridPos.y - monsterGridPos.y;

    // Prioritize directions based on distance to player
    std::vector<std::pair<Direction, float>> directions;

    // Calculate distance for each possible direction
    Vector2 testPos;
    float moveDistance = static_cast<float>(grid.getTileSize());

    // Up
    testPos = {position.x, position.y - moveDistance};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::UP, dist});
    }

    // Down
    testPos = {position.x, position.y + moveDistance};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::DOWN, dist});
    }

    // Left
    testPos = {position.x - moveDistance, position.y};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::LEFT, dist});
    }

    // Right
    testPos = {position.x + moveDistance, position.y};
    if (canMoveTo(testPos, grid))
    {
        Vector2 testGridPos = grid.worldToGrid(testPos);
        float dist = std::abs(playerGridPos.x - testGridPos.x) + std::abs(playerGridPos.y - testGridPos.y);
        directions.push_back({Direction::RIGHT, dist});
    }

    // Sort by distance (shortest first)
    std::sort(directions.begin(), directions.end(),
              [](const std::pair<Direction, float> &a, const std::pair<Direction, float> &b)
              {
                  return a.second < b.second;
              });

    // Return the best direction, or NONE if no valid directions
    return directions.empty() ? Direction::NONE : directions[0].first;
}

Direction RedMonster::findRandomValidDirection(const Grid &grid)
{
    std::vector<Direction> validDirections;
    float moveDistance = static_cast<float>(grid.getTileSize());

    // Check all directions
    std::vector<Direction> allDirections = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    for (Direction dir : allDirections)
    {
        Vector2 testPos = position;
        switch (dir)
        {
        case Direction::UP:
            testPos.y -= moveDistance;
            break;
        case Direction::DOWN:
            testPos.y += moveDistance;
            break;
        case Direction::LEFT:
            testPos.x -= moveDistance;
            break;
        case Direction::RIGHT:
            testPos.x += moveDistance;
            break;
        default:
            continue;
        }

        if (canMoveTo(testPos, grid))
        {
            validDirections.push_back(dir);
        }
    }

    if (validDirections.empty())
        return Direction::NONE;

    // Return random valid direction
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validDirections.size() - 1);

    return validDirections[dis(gen)];
}