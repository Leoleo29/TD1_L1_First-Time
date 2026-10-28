
#include "game.h"

bool keyCollected1[kMaxTutorialHeight][kMaxTutorialWidth] = {};
bool keyCollected2[kMaxStage1Height][kMaxStage1Width] = {};

void InitializeTutorialPlayer(Player& player)
{
    player.x = 1 * kMapSize + kMapSize / 2.0f;
    player.y = 18 * kMapSize + kMapSize / 2.0f;
    player.direction = 2;
    player.isMoving = false;
    player.speed = 10.0f;

    player.warpLocked = false;
    player.warpMapX = -1;
    player.warpMapY = -1;

    player.hp = kPlayerMaxHP;
    player.invincibleTimer = 0;
    player.keyCount = 0;
    player.showKeyArrow = false;
}

void InitializeStage1Player(Player& player)
{
    player.x = 1 * kMapSize + kMapSize / 2.0f;
    player.y = 38 * kMapSize + kMapSize / 2.0f;
    player.direction = 2;
    player.isMoving = false;
    player.speed = 10.0f;

    player.warpLocked = false;
    player.warpMapX = -1;
    player.warpMapY = -1;

    player.hp = kPlayerMaxHP;
    player.invincibleTimer = 0;
    player.keyCount = 0;
    player.showKeyArrow = false;
}

void InitializeMap()
{
    for (int y = 0; y < kMaxTutorialHeight; y++)
    {
        for (int x = 0; x < kMaxTutorialWidth; x++)
        {
            keyCollected1[y][x] = false;
        }
    }

    for (int y = 0; y < kMaxStage1Height; y++)
    {
        for (int x = 0; x < kMaxStage1Width; x++)
        {
            keyCollected2[y][x] = false;
        }
    }
}

template <int Height, int Width>
bool CanMove(
    float nextX,
    float nextY,
    int direction,
    const int(&map)[Height][Width]
)
{
    const float halfSize = kMapSize / 2.0f;

    float left = nextX - halfSize;
    float right = nextX + halfSize;
    float top = nextY - halfSize;
    float bottom = nextY + halfSize;

    int leftMapX = static_cast<int>(left) / kMapSize;
    int rightMapX = static_cast<int>(right - 1.0f) / kMapSize;
    int topMapY = static_cast<int>(top) / kMapSize;
    int bottomMapY = static_cast<int>(bottom - 1.0f) / kMapSize;

    for (int y = topMapY; y <= bottomMapY; y++)
    {
        for (int x = leftMapX; x <= rightMapX; x++)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height)
            {
                return false;
            }

            int tile = map[y][x];

            if (tile == 1) return false;
            if (tile == kBlockUp && direction != 0) return false;
            if (tile == kBlockRight && direction != 1) return false;
            if (tile == kBlockDown && direction != 2) return false;
            if (tile == kBlockLeft && direction != 3) return false;
        }
    }

    return true;
}

template <int Height, int Width>
void UpdateWarp(Player& player, const int(&map)[Height][Width])
{
    int playerMapX = static_cast<int>(player.x) / kMapSize;
    int playerMapY = static_cast<int>(player.y) / kMapSize;

    if (player.warpLocked &&
        (playerMapX != player.warpMapX || playerMapY != player.warpMapY))
    {
        player.warpLocked = false;
    }

    if (player.warpLocked) return;

    if (playerMapX < 0 || playerMapX >= Width ||
        playerMapY < 0 || playerMapY >= Height)
    {
        return;
    }

    if (map[playerMapY][playerMapX] != kBlockWarp) return;

    int warpX[Height * Width];
    int warpY[Height * Width];
    int warpCount = 0;

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            if (map[y][x] == kBlockWarp)
            {
                warpX[warpCount] = x;
                warpY[warpCount] = y;
                warpCount++;
            }
        }
    }

    if (warpCount < 2) return;

    int currentWarp = -1;

    for (int i = 0; i < warpCount; i++)
    {
        if (warpX[i] == playerMapX && warpY[i] == playerMapY)
        {
            currentWarp = i;
            break;
        }
    }

    if (currentWarp == -1) return;

    int destinationWarp =
        (currentWarp % 2 == 0) ? currentWarp + 1 : currentWarp - 1;

    if (destinationWarp >= warpCount) return;

    player.x = warpX[destinationWarp] * kMapSize + kMapSize / 2.0f;
    player.y = warpY[destinationWarp] * kMapSize + kMapSize / 2.0f;

    player.warpLocked = true;
    player.warpMapX = warpX[destinationWarp];
    player.warpMapY = warpY[destinationWarp];
}

template <int Height, int Width>
bool CheckSpikeCollision(const Player& player, const int(&map)[Height][Width])
{
    const float halfSize = kMapSize / 2.0f;

    float left = player.x - halfSize;
    float right = player.x + halfSize;
    float top = player.y - halfSize;
    float bottom = player.y + halfSize;

    int leftX = static_cast<int>(left) / kMapSize;
    int rightX = static_cast<int>(right - 1.0f) / kMapSize;
    int topY = static_cast<int>(top) / kMapSize;
    int bottomY = static_cast<int>(bottom - 1.0f) / kMapSize;

    for (int y = topY; y <= bottomY; y++)
    {
        for (int x = leftX; x <= rightX; x++)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height) continue;
            if (map[y][x] == kBlockSpike) return true;
        }
    }

    return false;
}

template <int Height, int Width>
void UpdateSpikeDamage(Player& player, const int(&map)[Height][Width])
{
    if (player.invincibleTimer > 0)
    {
        player.invincibleTimer--;
    }

    if (player.invincibleTimer > 0) return;
    if (!CheckSpikeCollision(player, map)) return;

    player.hp--;

    if (player.hp < 0) player.hp = 0;

    player.invincibleTimer = kInvincibleTime;
}

template <int Height, int Width>
void UpdateKeyCollection(
    Player& player,
    const int(&map)[Height][Width],
    bool(&collected)[Height][Width]
)
{
    int mapX = static_cast<int>(player.x) / kMapSize;
    int mapY = static_cast<int>(player.y) / kMapSize;

    if (mapX < 0 || mapX >= Width || mapY < 0 || mapY >= Height) return;

    if (map[mapY][mapX] == kBlockKey && !collected[mapY][mapX])
    {
        collected[mapY][mapX] = true;
        player.keyCount++;
    }
}

template <int Height, int Width>
bool CheckGoal(const Player& player, const int(&map)[Height][Width])
{
    if (player.keyCount < kRequiredKeyCount) return false;

    int mapX = static_cast<int>(player.x) / kMapSize;
    int mapY = static_cast<int>(player.y) / kMapSize;

    if (mapX < 0 || mapX >= Width || mapY < 0 || mapY >= Height) return false;

    return map[mapY][mapX] == kBlockGoal;
}

bool IsTutorialGoalReached(const Player& player)
{
    return CheckGoal(player, mapData1);
}

bool IsStage1GoalReached(const Player& player)
{
    return CheckGoal(player, mapData2);
}

template <int Height, int Width>
void UpdatePlayerCommon(
    Player& player,
    char* keys,
    char* preKeys,
    const int(&map)[Height][Width],
    bool(&collected)[Height][Width]
)
{
    if (preKeys[DIK_H] == 0 && keys[DIK_H] != 0)
    {
        player.showKeyArrow = !player.showKeyArrow;
    }

    if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
    {
        player.direction--;

        if (player.direction < 0) player.direction = 3;

        player.isMoving = true;
    }

    if (player.isMoving)
    {
        float nextX = player.x;
        float nextY = player.y;

        if (player.direction == 0) nextY -= player.speed;
        else if (player.direction == 1) nextX += player.speed;
        else if (player.direction == 2) nextY += player.speed;
        else nextX -= player.speed;

        if (!CanMove(nextX, nextY, player.direction, map))
        {
            player.isMoving = false;
        }
        else
        {
            player.x = nextX;
            player.y = nextY;
        }
    }

    UpdateWarp(player, map);
    UpdateKeyCollection(player, map, collected);
    UpdateSpikeDamage(player, map);
}

void UpdateTutorialPlayer(Player& player, char* keys, char* preKeys)
{
    UpdatePlayerCommon(player, keys, preKeys, mapData1, keyCollected1);
}

void UpdateStage1Player(Player& player, char* keys, char* preKeys)
{
    UpdatePlayerCommon(player, keys, preKeys, mapData2, keyCollected2);
}
