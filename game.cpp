#include "game.h"

// ゲーム全体の初期化
void InitializeGame(
    Player& player,
    Camera& camera
)
{
    // プレイヤー初期化
    InitializePlayer(player);

    // マップ初期化
    InitializeMap();

    // カメラ初期位置
    camera.position = { 0.0f, 0.0f };
}

// ゲーム全体の更新
void UpdateGame(
    Player& player,
    Camera& camera,
    char* keys,
    char* preKeys
)
{
    // プレイヤー更新
    UpdatePlayer(player, keys, preKeys);

    // カメラ更新
    UpdateCamera(camera, player);
}

// ゲーム全体の描画
void DrawGame(
    const Player& player,
    const Camera& camera,
    int blockGraphHandle
)
{
    // マップ描画
    DrawMap(blockGraphHandle, camera);

    // プレイヤー描画
    DrawPlayer(player, camera);

    // 向きの表示
    DrawDirectionBorder(player);
}

// カメラ更新
void UpdateCamera(
    Camera& camera,
    const Player& player
)
{
    camera.position.x =
        player.position.x - kWindowWidth / 2.0f;

    camera.position.y =
        player.position.y - kWindowHeight / 2.0f;

    // マップの外にカメラが出ないようにする
    const float maxCameraX =
        kMaxWidth * kMapSize - kWindowWidth;

    const float maxCameraY =
        kMaxHeight * kMapSize - kWindowHeight;

    if (camera.position.x < 0.0f)
    {
        camera.position.x = 0.0f;
    }

    if (camera.position.y < 0.0f)
    {
        camera.position.y = 0.0f;
    }

    if (camera.position.x > maxCameraX)
    {
        camera.position.x = maxCameraX;
    }

    if (camera.position.y > maxCameraY)
    {
        camera.position.y = maxCameraY;
    }
}