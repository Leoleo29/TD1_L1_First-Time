#include "game.h"

// プレイヤー初期化
void InitializeTutorialPlayer(Player& player)
{
    player.x = 1 * kMapSize + kMapSize / 2.0f;
    player.y = 18 * kMapSize + kMapSize / 2.0f;

    player.direction = 2;
    player.isMoving = false;
    player.speed = 10.0f;

    // ワープ設定
    player.warpLocked = false;
    player.warpMapX = -1;
    player.warpMapY = -1;

    // HP設定
    player.hp = kPlayerMaxHP;
    player.invincibleTimer = 0;
}

void InitializeStage1Player(Player& player)
{
    player.x = 1 * kMapSize + kMapSize / 2.0f;
    player.y = 38 * kMapSize + kMapSize / 2.0f;

    player.direction = 2;
    player.isMoving = false;
    player.speed = 10.0f;

    // ワープ設定
    player.warpLocked = false;
    player.warpMapX = -1;
    player.warpMapY = -1;

    // HP設定
    player.hp = kPlayerMaxHP;
    player.invincibleTimer = 0;
}

// 移動できるか判定
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
            if (x < 0 || x >= Width ||
                y < 0 || y >= Height)
            {
                return false;
            }

            int tile = map[y][x];

            // 壁
            if (tile == 1)
            {
                return false;
            }

            // 上方向の一方通行
            if (tile == kBlockUp && direction != 0)
            {
                return false;
            }

            // 右方向の一方通行
            if (tile == kBlockRight && direction != 1)
            {
                return false;
            }

            // 下方向の一方通行
            if (tile == kBlockDown && direction != 2)
            {
                return false;
            }

            // 左方向の一方通行
            if (tile == kBlockLeft && direction != 3)
            {
                return false;
            }

            // とげは通過できる
        }
    }

    return true;
}

// ワープ処理
template <int Height, int Width>
void UpdateWarp(
    Player& player,
    const int(&map)[Height][Width]
)
{
    int playerMapX = static_cast<int>(player.x) / kMapSize;
    int playerMapY = static_cast<int>(player.y) / kMapSize;

    // ワープ先のマスから出たら再ワープ可能
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

    int warpX[Height * Width];
    int warpY[Height * Width];
    int warpCount = 0;

    // ワープ地点を上の行から順番に集める
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

    // ワープ地点が2つ未満ならワープしない
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

    // ペアのないワープ地点は無効
    if (destinationWarp >= warpCount)
    {
        return;
    }

    // 移動先のマスの中央へ移動
    player.x =
        warpX[destinationWarp] * kMapSize
        + kMapSize / 2.0f;

    player.y =
        warpY[destinationWarp] * kMapSize
        + kMapSize / 2.0f;

    player.warpLocked = true;
    player.warpMapX = warpX[destinationWarp];
    player.warpMapY = warpY[destinationWarp];
}

// とげブロックとの当たり判定
template <int Height, int Width>
bool CheckSpikeCollision(
    const Player& player,
    const int(&map)[Height][Width]
)
{
    const float halfSize = kMapSize / 2.0f;

    float playerLeft = player.x - halfSize;
    float playerRight = player.x + halfSize;
    float playerTop = player.y - halfSize;
    float playerBottom = player.y + halfSize;

    int leftMapX =
        static_cast<int>(playerLeft) / kMapSize;

    int rightMapX =
        static_cast<int>(playerRight - 1.0f) / kMapSize;

    int topMapY =
        static_cast<int>(playerTop) / kMapSize;

    int bottomMapY =
        static_cast<int>(playerBottom - 1.0f) / kMapSize;

    for (int y = topMapY; y <= bottomMapY; y++)
    {
        for (int x = leftMapX; x <= rightMapX; x++)
        {
            if (x < 0 || x >= Width ||
                y < 0 || y >= Height)
            {
                continue;
            }

            if (map[y][x] == kBlockSpike)
            {
                return true;
            }
        }
    }

    return false;
}

// とげによるダメージ処理
template <int Height, int Width>
void UpdateSpikeDamage(
    Player& player,
    const int(&map)[Height][Width]
)
{
    // 無敵時間を減らす
    if (player.invincibleTimer > 0)
    {
        player.invincibleTimer--;
    }

    // すでに無敵時間中ならダメージを受けない
    if (player.invincibleTimer > 0)
    {
        return;
    }

    // とげに触れていなければ何もしない
    if (!CheckSpikeCollision(player, map))
    {
        return;
    }

    // HPを1減らす
    player.hp--;

    // HPが0未満にならないようにする
    if (player.hp < 0)
    {
        player.hp = 0;
    }

    // 無敵時間を開始
    player.invincibleTimer = kInvincibleTime;
}

// チュートリアル更新
void UpdateTutorialPlayer(
    Player& player,
    char* keys,
    char* preKeys
)
{
    // スペースキーで方向を変更
    if (preKeys[DIK_SPACE] == 0 &&
        keys[DIK_SPACE] != 0)
    {
        player.direction--;

        if (player.direction < 0)
        {
            player.direction = 3;
        }

        player.isMoving = true;
    }

    if (player.isMoving)
    {
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
        }
        else
        {
            player.x = nextX;
            player.y = nextY;
        }
    }

    // ワープ判定
    UpdateWarp(player, mapData1);

    // とげによるダメージ判定
    UpdateSpikeDamage(player, mapData1);
}

// ステージ1更新
void UpdateStage1Player(
    Player& player,
    char* keys,
    char* preKeys
)
{
    // スペースキーで方向を変更
    if (preKeys[DIK_SPACE] == 0 &&
        keys[DIK_SPACE] != 0)
    {
        player.direction--;

        if (player.direction < 0)
        {
            player.direction = 3;
        }

        player.isMoving = true;
    }

    if (player.isMoving)
    {
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
        }
        else
        {
            player.x = nextX;
            player.y = nextY;
        }
    }

    // ワープ判定
    UpdateWarp(player, mapData2);

    // とげによるダメージ判定
    UpdateSpikeDamage(player, mapData2);
}