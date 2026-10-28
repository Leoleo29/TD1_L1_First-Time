#include "game.h"
#include <math.h>

// 2つのマップで共有するワープアニメーションタイマー
static int warpAnimationTimer = 0;

void UpdateTutorialCamera(Camera& camera, const Player& player)
{
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    if (camera.x < 0.0f) camera.x = 0.0f;
    if (camera.y < 0.0f) camera.y = 0.0f;

    float maxX = kMaxTutorialWidth * kMapSize - kGameWidth;
    float maxY = kMaxTutorialHeight * kMapSize - kGameHeight;

    if (camera.x > maxX) camera.x = maxX;
    if (camera.y > maxY) camera.y = maxY;
}

void UpdateStage1Camera(Camera& camera, const Player& player)
{
    camera.x = player.x - kGameWidth / 2.0f;
    camera.y = player.y - kGameHeight / 2.0f;

    if (camera.x < 0.0f) camera.x = 0.0f;
    if (camera.y < 0.0f) camera.y = 0.0f;

    float maxX = kMaxStage1Width * kMapSize - kGameWidth;
    float maxY = kMaxStage1Height * kMapSize - kGameHeight;

    if (camera.x > maxX) camera.x = maxX;
    if (camera.y > maxY) camera.y = maxY;
}

template <int Height, int Width>
void DrawMapCommon(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    int warpTexture1,
    int warpTexture2,
    int warpTexture3,
    int warpTexture4,
    int trapTexture,
    int keyTexture,
    const Camera& camera,
    const int(&map)[Height][Width],
    bool(&collected)[Height][Width]
)
{
    // 8フレームごとにワープ画像を切り替える
    int warpFrame = (warpAnimationTimer / 8) % 4;

    int warpTextures[4] =
    {
        warpTexture1,
        warpTexture2,
        warpTexture3,
        warpTexture4
    };

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            int posX = kGameX + x * kMapSize
                - static_cast<int>(camera.x);

            int posY = kGameY + y * kMapSize
                - static_cast<int>(camera.y);

            // 画面外のマップチップは描画しない
            if (posX + kMapSize <= kGameX ||
                posX >= kGameX + kGameWidth ||
                posY + kMapSize <= kGameY ||
                posY >= kGameY + kGameHeight)
            {
                continue;
            }

            int texture = 0;

            switch (map[y][x])
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
                // 4枚の画像をアニメーション表示
                texture = warpTextures[warpFrame];
                break;

            case kBlockSpike:
                // トラップ画像
                texture = trapTexture;
                break;

            case kBlockKey:
                // 取得していない鍵だけ表示
                if (!collected[y][x])
                {
                    texture = keyTexture;
                }
                break;

            case kBlockGoal:
                // ゴール
                Novice::DrawBox(
                    posX + 5,
                    posY + 5,
                    kMapSize - 10,
                    kMapSize - 10,
                    0.0f,
                    0x33CC66FF,
                    kFillModeSolid
                );

                Novice::ScreenPrintf(
                    posX + 20,
                    posY + 32,
                    "GOAL"
                );
                break;

            default:
                break;
            }

            // 選択されたリソースを描画
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

void DrawTutorialMap(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    int warpTexture1,
    int warpTexture2,
    int warpTexture3,
    int warpTexture4,
    int trapTexture,
    int keyTexture,
    const Camera& camera
)
{
    DrawMapCommon(
        blockTexture,
        upBlock,
        rightBlock,
        downBlock,
        leftBlock,
        warpTexture1,
        warpTexture2,
        warpTexture3,
        warpTexture4,
        trapTexture,
        keyTexture,
        camera,
        mapData1,
        keyCollected1
    );

    // マップ描画ごとに1フレーム進める
    warpAnimationTimer++;
}

void DrawStage1Map(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    int warpTexture1,
    int warpTexture2,
    int warpTexture3,
    int warpTexture4,
    int trapTexture,
    int keyTexture,
    const Camera& camera
)
{
    DrawMapCommon(
        blockTexture,
        upBlock,
        rightBlock,
        downBlock,
        leftBlock,
        warpTexture1,
        warpTexture2,
        warpTexture3,
        warpTexture4,
        trapTexture,
        keyTexture,
        camera,
        mapData2,
        keyCollected2
    );

    // マップ描画ごとに1フレーム進める
    warpAnimationTimer++;
}

void DrawBackground(
    const Player& player,
    int bar1Texture,
    int bar2Texture,
    int heart1Texture,
    int heart2Texture,
    int heart3Texture,
    int heart4Texture
)
{
    // 画面の外側を黒くする
    Novice::DrawBox(
        0, 0, 1920, 140,
        0.0f, BLACK, kFillModeSolid
    );

    Novice::DrawBox(
        0, 140, 560, 800,
        0.0f, BLACK, kFillModeSolid
    );

    Novice::DrawBox(
        1360, 140, 560, 800,
        0.0f, BLACK, kFillModeSolid
    );

    Novice::DrawBox(
        0, 940, 1920, 140,
        0.0f, BLACK, kFillModeSolid
    );

    // プレイヤーの向きに合わせたバー
    if (player.direction == 0)
    {
        Novice::DrawSprite(
            560, 100, bar1Texture,
            1.0f, 1.0f, 0.0f, WHITE
        );
    }

    if (player.direction == 1)
    {
        Novice::DrawSprite(
            1360, 140, bar2Texture,
            1.0f, 1.0f, 0.0f, WHITE
        );
    }

    if (player.direction == 2)
    {
        Novice::DrawSprite(
            560, 940, bar1Texture,
            1.0f, 1.0f, 0.0f, WHITE
        );
    }

    if (player.direction == 3)
    {
        Novice::DrawSprite(
            520, 140, bar2Texture,
            1.0f, 1.0f, 0.0f, WHITE
        );
    }

    // HPに合わせてハート画像を切り替える
    int heartTexture = 0;

    if (player.hp >= 4)
    {
        heartTexture = heart1Texture;
    }
    else if (player.hp == 3)
    {
        heartTexture = heart2Texture;
    }
    else if (player.hp == 2)
    {
        heartTexture = heart3Texture;
    }
    else if (player.hp == 1)
    {
        heartTexture = heart4Texture;
    }

    // 左側のUIエリアにハートを表示
    if (heartTexture != 0)
    {
        Novice::DrawSprite(
            80, 160,
            heartTexture,
            1.0f, 1.0f,
            0.0f, WHITE
        );
    }
}

void DrawPlayer(
    const Player& player,
    const Camera& camera,
    int playerFrontTexture,
    int playerBackTexture,
    int playerLeftTexture,
    int playerRightTexture
)
{
    int drawX = kGameX + static_cast<int>(
        player.x - camera.x - kMapSize / 2.0f
        );

    int drawY = kGameY + static_cast<int>(
        player.y - camera.y - kMapSize / 2.0f
        );

    // 無敵時間中は点滅
    if (player.invincibleTimer > 0 &&
        (player.invincibleTimer / 5) % 2 == 0)
    {
        return;
    }

    int texture = playerFrontTexture;

    // direction の値に合わせて画像を切り替える
    switch (player.direction)
    {
    case 0: // 上
        texture = playerFrontTexture;
        break;

    case 1: // 右
        texture = playerRightTexture;
        break;

    case 2: // 下
        texture = playerBackTexture;
        break;

    case 3: // 左
        texture = playerLeftTexture;
        break;
    }

    Novice::DrawSprite(
        drawX,
        drawY,
        texture,
        1.0f,
        1.0f,
        0.0f,
        WHITE
    );
}

template <int Height, int Width>
void DrawNearestKeyArrowCommon(
    const Player& player,
    const Camera& camera,
    const int(&map)[Height][Width],
    bool(&collected)[Height][Width]
)
{
    if (!player.showKeyArrow)
    {
        return;
    }

    float nearestDistance = 1.0e30f;
    int nearestX = -1;
    int nearestY = -1;

    // 一番近い未取得の鍵を探す
    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            if (map[y][x] != kBlockKey || collected[y][x])
            {
                continue;
            }

            float dx =
                x * kMapSize + kMapSize / 2.0f - player.x;

            float dy =
                y * kMapSize + kMapSize / 2.0f - player.y;

            float distance = dx * dx + dy * dy;

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestX = x;
                nearestY = y;
            }
        }
    }

    if (nearestX < 0)
    {
        return;
    }

    float dx =
        nearestX * kMapSize + kMapSize / 2.0f - player.x;

    float dy =
        nearestY * kMapSize + kMapSize / 2.0f - player.y;

    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.0f)
    {
        return;
    }

    dx /= length;
    dy /= length;

    int centerX = kGameX
        + static_cast<int>(player.x - camera.x);

    int centerY = kGameY
        + static_cast<int>(player.y - camera.y);

    int baseX = centerX + static_cast<int>(dx * 80.0f);
    int baseY = centerY + static_cast<int>(dy * 80.0f);

    int tipX = baseX + static_cast<int>(dx * 22.0f);
    int tipY = baseY + static_cast<int>(dy * 22.0f);

    int leftX = baseX
        + static_cast<int>(-dx * 8.0f - dy * 11.0f);

    int leftY = baseY
        + static_cast<int>(-dy * 8.0f + dx * 11.0f);

    int rightX = baseX
        + static_cast<int>(-dx * 8.0f + dy * 11.0f);

    int rightY = baseY
        + static_cast<int>(-dy * 8.0f - dx * 11.0f);

    Novice::DrawTriangle(
        tipX, tipY,
        leftX, leftY,
        rightX, rightY,
        0xFFFF00FF,
        kFillModeSolid
    );
}

void DrawKeyArrow(
    const Player& player,
    const Camera& camera,
    bool isTutorial
)
{
    if (isTutorial)
    {
        DrawNearestKeyArrowCommon(
            player,
            camera,
            mapData1,
            keyCollected1
        );
    }
    else
    {
        DrawNearestKeyArrowCommon(
            player,
            camera,
            mapData2,
            keyCollected2
        );
    }
}