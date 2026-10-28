#include "game.h"

// 描画処理

#pragma region // チュートリアル

// チュートリアルのカメラ移動
void UpdateTutorialCamera(
    Camera& camera,
    const Player& player
)
{
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    if (camera.x < 0.0f)
    {
        camera.x = 0.0f;
    }

    if (camera.y < 0.0f)
    {
        camera.y = 0.0f;
    }

    float maxCameraX =
        kMaxTutorialWidth * kMapSize - kGameWidth;

    float maxCameraY =
        kMaxTutorialHeight * kMapSize - kGameHeight;

    if (camera.x > maxCameraX)
    {
        camera.x = maxCameraX;
    }

    if (camera.y > maxCameraY)
    {
        camera.y = maxCameraY;
    }
}

// チュートリアルマップの描画
void DrawTutorialMap(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    const Camera& camera
)
{
    for (int y = 0; y < kMaxTutorialHeight; y++)
    {
        for (int x = 0; x < kMaxTutorialWidth; x++)
        {
            int posX =
                kGameX + x * kMapSize
                - static_cast<int>(camera.x);

            int posY =
                kGameY + y * kMapSize
                - static_cast<int>(camera.y);

            if (
                posX + kMapSize <= kGameX ||
                posX >= kGameX + kGameWidth ||
                posY + kMapSize <= kGameY ||
                posY >= kGameY + kGameHeight
                )
            {
                continue;
            }

            int texture = 0;

            switch (mapData1[y][x])
            {
            case 1:
                texture = blockTexture;
                break;

            case kBlockUp:
                texture = upBlock;
                break;

            case kBlockRight:
                texture = rightBlock;
                break;

            case kBlockDown:
                texture = downBlock;
                break;

            case kBlockLeft:
                texture = leftBlock;
                break;

            case kBlockWarp:
                // ワープ地点
                Novice::DrawBox(
                    posX,
                    posY,
                    kMapSize,
                    kMapSize,
                    0.0f,
                    0xAA55FFFF,
                    kFillModeSolid
                );
                break;

            case kBlockSpike:
                // とげブロック
                Novice::DrawTriangle(
                    posX,
                    posY + kMapSize,
                    posX + kMapSize / 2,
                    posY,
                    posX + kMapSize,
                    posY + kMapSize,
                    0xFF3333FF,
                    kFillModeSolid
                );
                break;

            default:
                break;
            }

            if (texture != 0)
            {
                Novice::DrawSprite(
                    posX,
                    posY,
                    texture,
                    1.0f,
                    1.0f,
                    0.0f,
                    WHITE
                );
            }
        }
    }
}

#pragma endregion

#pragma region // ステージ1

// ステージ1のカメラ移動
void UpdateStage1Camera(
    Camera& camera,
    const Player& player
)
{
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    if (camera.x < 0.0f)
    {
        camera.x = 0.0f;
    }

    if (camera.y < 0.0f)
    {
        camera.y = 0.0f;
    }

    float maxCameraX =
        kMaxStage1Width * kMapSize - kGameWidth;

    float maxCameraY =
        kMaxStage1Height * kMapSize - kGameHeight;

    if (camera.x > maxCameraX)
    {
        camera.x = maxCameraX;
    }

    if (camera.y > maxCameraY)
    {
        camera.y = maxCameraY;
    }
}

// ステージ1マップの描画
void DrawStage1Map(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    const Camera& camera
)
{
    for (int y = 0; y < kMaxStage1Height; y++)
    {
        for (int x = 0; x < kMaxStage1Width; x++)
        {
            int posX =
                kGameX + x * kMapSize
                - static_cast<int>(camera.x);

            int posY =
                kGameY + y * kMapSize
                - static_cast<int>(camera.y);

            if (
                posX + kMapSize <= kGameX ||
                posX >= kGameX + kGameWidth ||
                posY + kMapSize <= kGameY ||
                posY >= kGameY + kGameHeight
                )
            {
                continue;
            }

            int texture = 0;

            switch (mapData2[y][x])
            {
            case 1:
                texture = blockTexture;
                break;

            case kBlockUp:
                texture = upBlock;
                break;

            case kBlockRight:
                texture = rightBlock;
                break;

            case kBlockDown:
                texture = downBlock;
                break;

            case kBlockLeft:
                texture = leftBlock;
                break;

            case kBlockWarp:
                // ワープ地点
                Novice::DrawBox(
                    posX,
                    posY,
                    kMapSize,
                    kMapSize,
                    0.0f,
                    0xAA55FFFF,
                    kFillModeSolid
                );
                break;

            case kBlockSpike:
                // とげブロック
                Novice::DrawTriangle(
                    posX,
                    posY + kMapSize,
                    posX + kMapSize / 2,
                    posY,
                    posX + kMapSize,
                    posY + kMapSize,
                    0xFF3333FF,
                    kFillModeSolid
                );
                break;

            default:
                break;
            }

            if (texture != 0)
            {
                Novice::DrawSprite(
                    posX,
                    posY,
                    texture,
                    1.0f,
                    1.0f,
                    0.0f,
                    WHITE
                );
            }
        }
    }
}

#pragma endregion

// 背景処理
void DrawBackground(
    const Player& player,
    int bar1Texture,
    int bar2Texture
)
{
    // 上
    Novice::DrawBox(
        0, 0, 1920, 140,
        0.0f, BLACK, kFillModeSolid
    );

    // 左
    Novice::DrawBox(
        0, 140, 560, 800,
        0.0f, BLACK, kFillModeSolid
    );

    // 右
    Novice::DrawBox(
        1360, 140, 560, 800,
        0.0f, BLACK, kFillModeSolid
    );

    // 下
    Novice::DrawBox(
        0, 940, 1920, 140,
        0.0f, BLACK, kFillModeSolid
    );

    // 上方向
    if (player.direction == 0)
    {
        Novice::DrawSprite(
            560, 100,
            bar1Texture,
            1.0f, 1.0f, 0.0f,
            WHITE
        );
    }

    // 右方向
    if (player.direction == 1)
    {
        Novice::DrawSprite(
            1360, 140,
            bar2Texture,
            1.0f, 1.0f, 0.0f,
            WHITE
        );
    }

    // 下方向
    if (player.direction == 2)
    {
        Novice::DrawSprite(
            560, 940,
            bar1Texture,
            1.0f, 1.0f, 0.0f,
            WHITE
        );
    }

    // 左方向
    if (player.direction == 3)
    {
        Novice::DrawSprite(
            520, 140,
            bar2Texture,
            1.0f, 1.0f, 0.0f,
            WHITE
        );
    }
}

// プレイヤーの描画処理
void DrawPlayer(
    const Player& player,
    const Camera& camera,
    int playerTexture
)
{
    int drawX =
        kGameX +
        static_cast<int>(
            player.x -
            camera.x -
            kMapSize / 2.0f
            );

    int drawY =
        kGameY +
        static_cast<int>(
            player.y -
            camera.y -
            kMapSize / 2.0f
            );

    // 無敵中は点滅させる
    if (player.invincibleTimer > 0 &&
        (player.invincibleTimer / 5) % 2 == 0)
    {
        return;
    }

    Novice::DrawSprite(
        drawX,
        drawY,
        playerTexture,
        1.0f,
        1.0f,
        0.0f,
        WHITE
    );
}