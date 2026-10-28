
#include "game.h"
#include <math.h>

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
    const Camera& camera,
    const int(&map)[Height][Width],
    bool(&collected)[Height][Width]
)
{
    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            int posX = kGameX + x * kMapSize - static_cast<int>(camera.x);
            int posY = kGameY + y * kMapSize - static_cast<int>(camera.y);

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
                Novice::DrawBox(
                    posX, posY, kMapSize, kMapSize,
                    0.0f, 0xAA55FFFF, kFillModeSolid
                );
                break;

            case kBlockSpike:
                Novice::DrawTriangle(
                    posX, posY + kMapSize,
                    posX + kMapSize / 2, posY,
                    posX + kMapSize, posY + kMapSize,
                    0xFF3333FF, kFillModeSolid
                );
                break;

            case kBlockKey:
                if (!collected[y][x])
                {
                    // 鍵の輪
                    Novice::DrawEllipse(
                        posX + 30, posY + 18,
                        12, 12, 0.0f,
                        0xFFD700FF, kFillModeSolid
                    );

                    Novice::DrawEllipse(
                        posX + 30, posY + 18,
                        5, 5, 0.0f,
                        0x333333FF, kFillModeSolid
                    );

                    // 鍵の棒
                    Novice::DrawBox(
                        posX + 38, posY + 28,
                        7, 30, 0.0f,
                        0xFFD700FF, kFillModeSolid
                    );

                    Novice::DrawBox(
                        posX + 42, posY + 48,
                        10, 5, 0.0f,
                        0xFFD700FF, kFillModeSolid
                    );
                }
                break;

            case kBlockGoal:
                Novice::DrawBox(
                    posX + 5, posY + 5,
                    kMapSize - 10, kMapSize - 10,
                    0.0f, 0x33CC66FF, kFillModeSolid
                );

                Novice::ScreenPrintf(
                    posX + 20, posY + 32, "GOAL"
                );
                break;

            default:
                break;
            }

            if (texture != 0)
            {
                Novice::DrawSprite(
                    posX, posY, texture,
                    1.0f, 1.0f, 0.0f, WHITE
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
    const Camera& camera
)
{
    DrawMapCommon(
        blockTexture, upBlock, rightBlock, downBlock, leftBlock,
        camera, mapData1, keyCollected1
    );
}

void DrawStage1Map(
    int blockTexture,
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    const Camera& camera
)
{
    DrawMapCommon(
        blockTexture, upBlock, rightBlock, downBlock, leftBlock,
        camera, mapData2, keyCollected2
    );
}

void DrawBackground(const Player& player, int bar1Texture, int bar2Texture)
{
    Novice::DrawBox(0, 0, 1920, 140, 0.0f, BLACK, kFillModeSolid);
    Novice::DrawBox(0, 140, 560, 800, 0.0f, BLACK, kFillModeSolid);
    Novice::DrawBox(1360, 140, 560, 800, 0.0f, BLACK, kFillModeSolid);
    Novice::DrawBox(0, 940, 1920, 140, 0.0f, BLACK, kFillModeSolid);

    if (player.direction == 0)
    {
        Novice::DrawSprite(560, 100, bar1Texture, 1.0f, 1.0f, 0.0f, WHITE);
    }

    if (player.direction == 1)
    {
        Novice::DrawSprite(1360, 140, bar2Texture, 1.0f, 1.0f, 0.0f, WHITE);
    }

    if (player.direction == 2)
    {
        Novice::DrawSprite(560, 940, bar1Texture, 1.0f, 1.0f, 0.0f, WHITE);
    }

    if (player.direction == 3)
    {
        Novice::DrawSprite(520, 140, bar2Texture, 1.0f, 1.0f, 0.0f, WHITE);
    }
}

void DrawPlayer(const Player& player, const Camera& camera, int playerTexture)
{
    int drawX = kGameX + static_cast<int>(
        player.x - camera.x - kMapSize / 2.0f
        );

    int drawY = kGameY + static_cast<int>(
        player.y - camera.y - kMapSize / 2.0f
        );

    if (player.invincibleTimer > 0 &&
        (player.invincibleTimer / 5) % 2 == 0)
    {
        return;
    }

    Novice::DrawSprite(
        drawX, drawY, playerTexture,
        1.0f, 1.0f, 0.0f, WHITE
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
    if (!player.showKeyArrow) return;

    float nearestDistance = 1.0e30f;
    int nearestX = -1;
    int nearestY = -1;

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            if (map[y][x] != kBlockKey || collected[y][x]) continue;

            float dx = x * kMapSize + kMapSize / 2.0f - player.x;
            float dy = y * kMapSize + kMapSize / 2.0f - player.y;
            float distance = dx * dx + dy * dy;

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestX = x;
                nearestY = y;
            }
        }
    }

    if (nearestX < 0) return;

    float dx = nearestX * kMapSize + kMapSize / 2.0f - player.x;
    float dy = nearestY * kMapSize + kMapSize / 2.0f - player.y;
    float length = sqrtf(dx * dx + dy * dy);

    if (length <= 0.0f) return;

    dx /= length;
    dy /= length;

    int centerX = kGameX + static_cast<int>(player.x - camera.x);
    int centerY = kGameY + static_cast<int>(player.y - camera.y);

    int baseX = centerX + static_cast<int>(dx * 80.0f);
    int baseY = centerY + static_cast<int>(dy * 80.0f);

    int tipX = baseX + static_cast<int>(dx * 22.0f);
    int tipY = baseY + static_cast<int>(dy * 22.0f);

    int leftX = baseX + static_cast<int>(-dx * 8.0f - dy * 11.0f);
    int leftY = baseY + static_cast<int>(-dy * 8.0f + dx * 11.0f);

    int rightX = baseX + static_cast<int>(-dx * 8.0f + dy * 11.0f);
    int rightY = baseY + static_cast<int>(-dy * 8.0f - dx * 11.0f);

    Novice::DrawTriangle(
        tipX, tipY, leftX, leftY, rightX, rightY,
        0xFFFF00FF, kFillModeSolid
    );
}

void DrawKeyArrow(const Player& player, const Camera& camera, bool isTutorial)
{
    if (isTutorial)
    {
        DrawNearestKeyArrowCommon(
            player, camera, mapData1, keyCollected1
        );
    }
    else
    {
        DrawNearestKeyArrowCommon(
            player, camera, mapData2, keyCollected2
        );
    }
}
