
#include "game.h"

// プレイヤー初期化
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
}

// 移動できるか判定
template <int Width, int Height>
bool CanMove(
    float nextX,
    float nextY,
    int direction,
    const int(&map)[Width][Height]
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

            if (tile == 1)
            {
                return false;
            }

            if (tile == kBlockUp && direction != 0)
            {
                return false;
            }

            if (tile == kBlockRight && direction != 1)
            {
                return false;
            }

            if (tile == kBlockDown && direction != 2)
            {
                return false;
            }

            if (tile == kBlockLeft && direction != 3)
            {
                return false;
            }
        }
    }

    return true;
}

// ワープ処理
template <int Width, int Height>
void UpdateWarp(
    Player& player,
    const int(&map)[Width][Height]
)
{
    int playerMapX = static_cast<int>(player.x) / kMapSize;
    int playerMapY = static_cast<int>(player.y) / kMapSize;

    // 移動先のマスから出たら再ワープ可能
    if (player.warpLocked)
    {
        if (playerMapX != player.warpMapX ||
            playerMapY != player.warpMapY)
        {
            player.warpLocked = false;
        }
    }

    if (player.warpLocked)
    {
        return;
    }

    if (playerMapX < 0 || playerMapX >= Width ||
        playerMapY < 0 || playerMapY >= Height)
    {
        return;
    }

    if (map[playerMapY][playerMapX] != kBlockWarp)
    {
        return;
    }

    int warpX[Width * Height];
    int warpY[Width * Height];
    int warpCount = 0;

    // 6の座標を上の行から順番に集める
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

    // ペアが作れない場合はワープしない
    if (warpCount < 2)
    {
        return;
    }

    int currentWarp = -1;

    for (int i = 0; i < warpCount; i++)
    {
        if (warpX[i] == playerMapX &&
            warpY[i] == playerMapY)
        {
            currentWarp = i;
            break;
        }
    }

    if (currentWarp == -1)
    {
        return;
    }

    // 0番と1番、2番と3番……をペアにする
    int destinationWarp;

    if (currentWarp % 2 == 0)
    {
        destinationWarp = currentWarp + 1;
    }
    else
    {
        destinationWarp = currentWarp - 1;
    }

    // ペアのない最後のワープは無効
    if (destinationWarp >= warpCount)
    {
        return;
    }

    // 移動先のマスの中央へ移動
    player.x = warpX[destinationWarp] * kMapSize
        + kMapSize / 2.0f;

    player.y = warpY[destinationWarp] * kMapSize
        + kMapSize / 2.0f;

    player.warpLocked = true;
    player.warpMapX = warpX[destinationWarp];
    player.warpMapY = warpY[destinationWarp];
}

// チュートリアル更新
void UpdateTutorialPlayer(
    Player& player,
    char* keys,
    char* preKeys
)
{
    if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
    {
        player.direction--;

        if (player.direction < 0)
        {
            player.direction = 3;
        }

        player.isMoving = true;
    }

    if (!player.isMoving)
    {
        UpdateWarp(player, mapData1);
        return;
    }

    float nextX = player.x;
    float nextY = player.y;

    if (player.direction == 0)
    {
        nextY -= player.speed;
    }
    else if (player.direction == 1)
    {
        nextX += player.speed;
    }
    else if (player.direction == 2)
    {
        nextY += player.speed;
    }
    else
    {
        nextX -= player.speed;
    }

    if (!CanMove(nextX, nextY, player.direction, mapData1))
    {
        player.isMoving = false;
        return;
    }

    player.x = nextX;
    player.y = nextY;

    UpdateWarp(player, mapData1);
}

// ステージ1更新
void UpdateStage1Player(
    Player& player,
    char* keys,
    char* preKeys
)
{
    if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
    {
        player.direction--;

        if (player.direction < 0)
        {
            player.direction = 3;
        }

        player.isMoving = true;
    }

    if (!player.isMoving)
    {
        UpdateWarp(player, mapData2);
        return;
    }

    float nextX = player.x;
    float nextY = player.y;

    if (player.direction == 0)
    {
        nextY -= player.speed;
    }
    else if (player.direction == 1)
    {
        nextX += player.speed;
    }
    else if (player.direction == 2)
    {
        nextY += player.speed;
    }
    else
    {
        nextX -= player.speed;
    }

    if (!CanMove(nextX, nextY, player.direction, mapData2))
    {
        player.isMoving = false;
        return;
    }

    player.x = nextX;
    player.y = nextY;

    UpdateWarp(player, mapData2);
}
