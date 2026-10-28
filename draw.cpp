
#include "game.h"

// 描画処理

#pragma region // チュートリアル

// チュートリアルのカメラ移動
void UpdateTutorialCamera(
    Camera& camera,
    const Player& player
)
{
    // プレイヤーを画面中央に配置
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    // 左端
    if (camera.x < 0.0f)
    {
        camera.x = 0.0f;
    }

    // 上端
    if (camera.y < 0.0f)
    {
        camera.y = 0.0f;
    }

    // カメラが移動できる最大位置
    float maxCameraX =
        kMaxTutorialWidth * kMapSize - kGameWidth;

    float maxCameraY =
        kMaxTutorialHeight * kMapSize - kGameHeight;

    // 右端
    if (camera.x > maxCameraX)
    {
        camera.x = maxCameraX;
    }

    // 下端
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

            // 画面外のマップチップは描画しない
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
                // 壁
                texture = blockTexture;
                break;

            case kBlockUp:
                // 上方向の一方通行
                texture = upBlock;
                break;

            case kBlockRight:
                // 右方向の一方通行
                texture = rightBlock;
                break;

            case kBlockDown:
                // 下方向の一方通行
                texture = downBlock;
                break;

            case kBlockLeft:
                // 左方向の一方通行
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

            default:
                // 床など、何も描画しないマス
                break;
            }

            // テクスチャがある場合は描画
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
    // プレイヤーを画面中央に配置
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    // 左端
    if (camera.x < 0.0f)
    {
        camera.x = 0.0f;
    }

    // 上端
    if (camera.y < 0.0f)
    {
        camera.y = 0.0f;
    }

    // カメラが移動できる最大位置
    float maxCameraX =
        kMaxStage1Width * kMapSize - kGameWidth;

    float maxCameraY =
        kMaxStage1Height * kMapSize - kGameHeight;

    // 右端
    if (camera.x > maxCameraX)
    {
        camera.x = maxCameraX;
    }

    // 下端
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

            // 画面外のマップチップは描画しない
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
                // 壁
                texture = blockTexture;
                break;

            case kBlockUp:
                // 上方向の一方通行
                texture = upBlock;
                break;

            case kBlockRight:
                // 右方向の一方通行
                texture = rightBlock;
                break;

            case kBlockDown:
                // 下方向の一方通行
                texture = downBlock;
                break;

            case kBlockLeft:
                // 左方向の一方通行
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

            default:
                // 床など、何も描画しないマス
                break;
            }

            // テクスチャがある場合は描画
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
