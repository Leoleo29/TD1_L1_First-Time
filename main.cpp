
#include <Novice.h>
#include <Windows.h>
#include <cstring>
#include "game.h"
#include "fullScreen.h"

const char kWindowTitle[] = "台パンするキンタロウ・ホズミ  ゼン";

int WINAPI WinMain(
    _In_ HINSTANCE,
    _In_opt_ HINSTANCE,
    _In_ LPSTR,
    _In_ int
)
{
    Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

    HWND gameWindow = GetActiveWindow();
    RECT windowRect = {};
    LONG_PTR windowStyle = 0;
    bool isFullScreen = false;

    if (gameWindow == nullptr)
    {
        OutputDebugStringA("gameWindow is nullptr\n");
    }

    int blockTexture = Novice::LoadTexture("./images/block.png");
    int playerTexture = Novice::LoadTexture("./images/player.png");
    int bar1Texture = Novice::LoadTexture("./images/bar1.png");
    int bar2Texture = Novice::LoadTexture("./images/bar2.png");

    int upBlock = Novice::LoadTexture("./images/oneWay_up.png");
    int rightBlock = Novice::LoadTexture("./images/oneWay_right.png");
    int downBlock = Novice::LoadTexture("./images/oneWay_down.png");
    int leftBlock = Novice::LoadTexture("./images/oneWay_left.png");

    int titleTexture = Novice::LoadTexture("./images/SCENE_Title.png");
    int clearTexture = Novice::LoadTexture("./images/SCENE_Clear.png");
    int gameOverTexture = Novice::LoadTexture("./images/SCENE_GameOver.png");

    char keys[256] = {};
    char preKeys[256] = {};

    Player player = {};
    InitializeTutorialPlayer(player);

    Camera camera = { 0.0f, 0.0f };
    InitializeMap();

    Scene scene = kSceneTitle;

    while (Novice::ProcessMessage() == 0)
    {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        if (keys[DIK_F11] != 0 && preKeys[DIK_F11] == 0)
        {
            ToggleFullScreen(
                gameWindow, windowRect, windowStyle, isFullScreen
            );
        }

        switch (scene)
        {
        case kSceneTitle:
            Novice::DrawSprite(
                0, 0, titleTexture,
                1.0f, 1.0f, 0.0f, WHITE
            );

            if ((preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0) ||
                (preKeys[DIK_1] == 0 && keys[DIK_1] != 0))
            {
                InitializeMap();
                InitializeTutorialPlayer(player);
                camera = { 0.0f, 0.0f };
                scene = kSceneTutorial;
            }

            if (preKeys[DIK_2] == 0 && keys[DIK_2] != 0)
            {
                InitializeMap();
                InitializeStage1Player(player);
                camera = { 0.0f, 0.0f };
                scene = kSceneStage1;
            }
            break;

        case kSceneTutorial:
            UpdateTutorialPlayer(player, keys, preKeys);

            if (player.hp <= 0)
            {
                scene = kSceneGameOver;
                break;
            }

            if (IsTutorialGoalReached(player))
            {
                scene = kSceneClear;
                break;
            }

            UpdateTutorialCamera(camera, player);

            DrawTutorialMap(
                blockTexture, upBlock, rightBlock,
                downBlock, leftBlock, camera
            );

            DrawPlayer(player, camera, playerTexture);
            DrawKeyArrow(player, camera, true);
            DrawBackground(player, bar1Texture, bar2Texture);

            Novice::ScreenPrintf(
                kGameX + 20, 50,
                "HP: %d / %d", player.hp, kPlayerMaxHP
            );

            Novice::ScreenPrintf(
                kGameX + 200, 50,
                "Invincible: %d", player.invincibleTimer
            );

            Novice::ScreenPrintf(
                kGameX + 400, 50,
                "KEY: %d / %d", player.keyCount, kRequiredKeyCount
            );

            Novice::ScreenPrintf(
                kGameX + 580, 50,
                "H: KEY ARROW"
            );
            break;

        case kSceneStage1:
            UpdateStage1Player(player, keys, preKeys);

            if (player.hp <= 0)
            {
                scene = kSceneGameOver;
                break;
            }

            if (IsStage1GoalReached(player))
            {
                scene = kSceneClear;
                break;
            }

            UpdateStage1Camera(camera, player);

            DrawStage1Map(
                blockTexture, upBlock, rightBlock,
                downBlock, leftBlock, camera
            );

            DrawPlayer(player, camera, playerTexture);
            DrawKeyArrow(player, camera, false);
            DrawBackground(player, bar1Texture, bar2Texture);

            Novice::ScreenPrintf(
                kGameX + 20, 50,
                "HP: %d / %d", player.hp, kPlayerMaxHP
            );

            Novice::ScreenPrintf(
                kGameX + 200, 50,
                "Invincible: %d", player.invincibleTimer
            );

            Novice::ScreenPrintf(
                kGameX + 400, 50,
                "KEY: %d / %d", player.keyCount, kRequiredKeyCount
            );

            Novice::ScreenPrintf(
                kGameX + 580, 50,
                "H: KEY ARROW"
            );
            break;

        case kSceneClear:
            Novice::DrawSprite(
                0, 0, clearTexture,
                1.0f, 1.0f, 0.0f, WHITE
            );

            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
            {
                scene = kSceneTitle;
            }
            break;

        case kSceneGameOver:
            Novice::DrawSprite(
                0, 0, gameOverTexture,
                1.0f, 1.0f, 0.0f, WHITE
            );

            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
            {
                scene = kSceneTitle;
            }
            break;
        }

        Novice::ScreenPrintf(
            10, 10, "full screen : %d",
            isFullScreen ? 1 : 0
        );

        Novice::ScreenPrintf(
            10, 30, "window handle : %d",
            gameWindow != nullptr ? 1 : 0
        );

        Novice::EndFrame();

        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
        {
            break;
        }
    }

    Novice::Finalize();
    return 0;
}
