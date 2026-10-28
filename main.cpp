
#include <Novice.h>
#include <Windows.h>
#include <cstring>
#include <cmath>
#include "game.h"
#include "fullScreen.h"

const char kWindowTitle[] = "台パンするキンタロウ・ホズミ  ゼン";

// BGMを切り替える
static int currentBgmHandle = -1;
static int currentBgmVoice = -1;
static Scene previousBgmScene = kSceneGameOver;
static bool hasStartedBgm = false;

void UpdateBGM(
    Scene scene,
    int titleBGM,
    int playBGM,
    int stage1BGM,
    int clearBGM,
    int gameOverBGM
)
{
    if (hasStartedBgm && scene == previousBgmScene)
    {
        return;
    }

    // 前のBGMを停止
    if (currentBgmVoice != -1 &&
        Novice::IsPlayingAudio(currentBgmVoice))
    {
        Novice::StopAudio(currentBgmVoice);
    }

    switch (scene)
    {
    case kSceneTitle:
        currentBgmHandle = titleBGM;
        break;

    case kSceneTutorial:
        currentBgmHandle = playBGM;
        break;

    case kSceneStage1:
        currentBgmHandle = stage1BGM;
        break;

    case kSceneClear:
        currentBgmHandle = clearBGM;
        break;

    case kSceneGameOver:
        currentBgmHandle = gameOverBGM;
        break;
    }

    // BGMはループ再生
    currentBgmVoice =
        Novice::PlayAudio(currentBgmHandle, true, 0.5f);

    previousBgmScene = scene;
    hasStartedBgm = true;
}

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

    // 基本リソース
    int blockTexture = Novice::LoadTexture("./Resources/images/block.png");
    int bar1Texture = Novice::LoadTexture("./Resources/images/bar1.png");
    int bar2Texture = Novice::LoadTexture("./Resources/images/bar2.png");

    // 一方通行ブロック
    int upBlock = Novice::LoadTexture("./Resources/images/oneWay_up.png");
    int rightBlock = Novice::LoadTexture("./Resources/images/oneWay_right.png");
    int downBlock = Novice::LoadTexture("./Resources/images/oneWay_down.png");
    int leftBlock = Novice::LoadTexture("./Resources/images/oneWay_left.png");

    // ワープ画像
    int warpTexture1 = Novice::LoadTexture("./Resources/images/warp1.png");
    int warpTexture2 = Novice::LoadTexture("./Resources/images/warp2.png");
    int warpTexture3 = Novice::LoadTexture("./Resources/images/warp3.png");
    int warpTexture4 = Novice::LoadTexture("./Resources/images/warp4.png");

    // トラップ・鍵
    int trapTexture = Novice::LoadTexture("./Resources/images/spikeTrap.png");
    int keyTexture = Novice::LoadTexture("./Resources/images/key.png");

    // ハートUI
    int heart1Texture = Novice::LoadTexture("./Resources/images/heart_UI_1.png");
    int heart2Texture = Novice::LoadTexture("./Resources/images/heart_UI_2.png");
    int heart3Texture = Novice::LoadTexture("./Resources/images/heart_UI_3.png");
    int heart4Texture = Novice::LoadTexture("./Resources/images/heart_UI_4.png");

    // シーン画像
    int titleTexture = Novice::LoadTexture("./Resources/images/SCENE_Title.png");
    int clearTexture = Novice::LoadTexture("./Resources/images/SCENE_Clear.png");
    int gameOverTexture = Novice::LoadTexture("./Resources/images/SCENE_GameOver.png");

    // プレイヤー4方向
    int playerFrontTexture = Novice::LoadTexture("./Resources/images/playerFront.png");
    int playerBackTexture = Novice::LoadTexture("./Resources/images/playerBack.png");
    int playerLeftTexture = Novice::LoadTexture("./Resources/images/playerLeft.png");
    int playerRightTexture = Novice::LoadTexture("./Resources/images/playerRight.png");

    // BGM
    const int kTitleBGMHandle =
        Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-173.mp3");

    const int kPlayHandle =
        Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-155.mp3");

    const int kStage1Handle =
        Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-112.mp3");

    const int kClearHandle =
        Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-156.mp3");

    const int kGameOverHandle =
        Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-110.mp3");

    // 効果音
    const int kTrapHandle =
        Novice::LoadAudio("./Resources/Sounds/damage.wav");

    const int kWarpHandle =
        Novice::LoadAudio("./Resources/Sounds/Motion-Slide07-1(Mid).mp3");

    const int kTruckHandle =
        Novice::LoadAudio("./Resources/Sounds/Footsteps03-2L.mp3");

    const int kKeyHandle =
        Novice::LoadAudio("./Resources/Sounds/Onoma-Sparkle12-1(High).mp3");

    const int kGoalHandle =
        Novice::LoadAudio("./Resources/Sounds/Onoma-Sparkle10-3(Long).mp3");

    char keys[256] = {};
    char preKeys[256] = {};

    Player player = {};
    InitializeTutorialPlayer(player);

    Camera camera = { 0.0f, 0.0f };
    InitializeMap();

    Scene scene = kSceneTitle;

    // 足音の再生ハンドル
    int footstepVoice = -1;

    // ゴール音の二重再生防止
    bool goalSoundPlayed = false;

    while (Novice::ProcessMessage() == 0)
    {
        Novice::BeginFrame();

        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        // F11でフルスクリーン切り替え
        if (keys[DIK_F11] != 0 && preKeys[DIK_F11] == 0)
        {
            ToggleFullScreen(
                gameWindow,
                windowRect,
                windowStyle,
                isFullScreen
            );
        }

        // シーンに応じてBGMを切り替える
        UpdateBGM(
            scene,
            kTitleBGMHandle,
            kPlayHandle,
            kStage1Handle,
            kClearHandle,
            kGameOverHandle
        );

        // プレイ画面以外では足音を停止
        if (scene != kSceneTutorial && scene != kSceneStage1 &&
            footstepVoice != -1)
        {
            if (Novice::IsPlayingAudio(footstepVoice))
            {
                Novice::StopAudio(footstepVoice);
            }

            footstepVoice = -1;
        }

        switch (scene)
        {
        case kSceneTitle:
        {
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
                goalSoundPlayed = false;
                scene = kSceneTutorial;
            }

            if (preKeys[DIK_2] == 0 && keys[DIK_2] != 0)
            {
                InitializeMap();
                InitializeStage1Player(player);
                camera = { 0.0f, 0.0f };
                goalSoundPlayed = false;
                scene = kSceneStage1;
            }

            break;
        }

        case kSceneTutorial:
        {
            // 移動前の状態を保存
            float oldX = player.x;
            float oldY = player.y;
            int oldHP = player.hp;
            int oldKeyCount = player.keyCount;

            UpdateTutorialPlayer(player, keys, preKeys);

            // ダメージ音は一度だけ再生
            if (player.hp < oldHP)
            {
                Novice::PlayAudio(kTrapHandle, false, 0.7f);
            }

            // 鍵取得音は一度だけ再生
            if (player.keyCount > oldKeyCount)
            {
                Novice::PlayAudio(kKeyHandle, false, 0.7f);
            }

            // ワープ判定
            float dx = player.x - oldX;
            float dy = player.y - oldY;

            bool didWarp =
                (dx * dx + dy * dy > 80.0f * 80.0f);

            if (didWarp)
            {
                // ワープ音は一度だけ再生
                Novice::PlayAudio(kWarpHandle, false, 0.7f);

                // ワープ時は足音を停止
                if (footstepVoice != -1)
                {
                    if (Novice::IsPlayingAudio(footstepVoice))
                    {
                        Novice::StopAudio(footstepVoice);
                    }

                    footstepVoice = -1;
                }
            }
            else if (player.isMoving)
            {
                // 足音は移動中ずっとループ再生
                if (footstepVoice == -1 ||
                    !Novice::IsPlayingAudio(footstepVoice))
                {
                    footstepVoice =
                        Novice::PlayAudio(kTruckHandle, true, 0.4f);
                }
            }
            else if (footstepVoice != -1)
            {
                // 移動が終わったら足音を停止
                if (Novice::IsPlayingAudio(footstepVoice))
                {
                    Novice::StopAudio(footstepVoice);
                }

                footstepVoice = -1;
            }

            if (player.hp <= 0)
            {
                scene = kSceneGameOver;
                break;
            }

            if (IsTutorialGoalReached(player))
            {
                if (!goalSoundPlayed)
                {
                    // ゴール音は一度だけ再生
                    Novice::PlayAudio(kGoalHandle, false, 0.7f);
                    goalSoundPlayed = true;
                }

                // チュートリアルのゴール後はステージ1へ
                scene = kSceneStage1;
                InitializeStage1Player(player);
                camera = { 0.0f, 0.0f };
                goalSoundPlayed = false;
                break;
            }

            UpdateTutorialCamera(camera, player);

            DrawTutorialMap(
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
                camera
            );

            DrawPlayer(
                player,
                camera,
                playerFrontTexture,
                playerBackTexture,
                playerLeftTexture,
                playerRightTexture
            );

            DrawKeyArrow(player, camera, true);

            DrawBackground(
                player,
                bar1Texture,
                bar2Texture,
                heart1Texture,
                heart2Texture,
                heart3Texture,
                heart4Texture
            );

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
        }

        case kSceneStage1:
        {
            float oldX = player.x;
            float oldY = player.y;
            int oldHP = player.hp;
            int oldKeyCount = player.keyCount;

            UpdateStage1Player(player, keys, preKeys);

            // ダメージ音は一度だけ再生
            if (player.hp < oldHP)
            {
                Novice::PlayAudio(kTrapHandle, false, 0.7f);
            }

            // 鍵取得音は一度だけ再生
            if (player.keyCount > oldKeyCount)
            {
                Novice::PlayAudio(kKeyHandle, false, 0.7f);
            }

            // ワープ判定
            float dx = player.x - oldX;
            float dy = player.y - oldY;

            bool didWarp =
                (dx * dx + dy * dy > 80.0f * 80.0f);

            if (didWarp)
            {
                // ワープ音は一度だけ再生
                Novice::PlayAudio(kWarpHandle, false, 0.7f);

                // ワープ時は足音を停止
                if (footstepVoice != -1)
                {
                    if (Novice::IsPlayingAudio(footstepVoice))
                    {
                        Novice::StopAudio(footstepVoice);
                    }

                    footstepVoice = -1;
                }
            }
            else if (player.isMoving)
            {
                // 足音は移動中ずっとループ再生
                if (footstepVoice == -1 ||
                    !Novice::IsPlayingAudio(footstepVoice))
                {
                    footstepVoice =
                        Novice::PlayAudio(kTruckHandle, true, 0.4f);
                }
            }
            else if (footstepVoice != -1)
            {
                // 移動が終わったら足音を停止
                if (Novice::IsPlayingAudio(footstepVoice))
                {
                    Novice::StopAudio(footstepVoice);
                }

                footstepVoice = -1;
            }

            if (player.hp <= 0)
            {
                scene = kSceneGameOver;
                break;
            }

            if (IsStage1GoalReached(player))
            {
                if (!goalSoundPlayed)
                {
                    // ゴール音は一度だけ再生
                    Novice::PlayAudio(kGoalHandle, false, 0.7f);
                    goalSoundPlayed = true;
                }

                scene = kSceneClear;
                break;
            }

            UpdateStage1Camera(camera, player);

            DrawStage1Map(
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
                camera
            );

            DrawPlayer(
                player,
                camera,
                playerFrontTexture,
                playerBackTexture,
                playerLeftTexture,
                playerRightTexture
            );

            DrawKeyArrow(player, camera, false);

            DrawBackground(
                player,
                bar1Texture,
                bar2Texture,
                heart1Texture,
                heart2Texture,
                heart3Texture,
                heart4Texture
            );

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
        }

        case kSceneClear:
        {
            Novice::DrawSprite(
                0, 0, clearTexture,
                1.0f, 1.0f, 0.0f, WHITE
            );

            if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
            {
                scene = kSceneTitle;
            }

            break;
        }

        case kSceneGameOver:
        {
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

    // 終了時に足音を停止
    if (footstepVoice != -1 &&
        Novice::IsPlayingAudio(footstepVoice))
    {
        Novice::StopAudio(footstepVoice);
    }

    // 終了時にBGMを停止
    if (currentBgmVoice != -1 &&
        Novice::IsPlayingAudio(currentBgmVoice))
    {
        Novice::StopAudio(currentBgmVoice);
    }

    Novice::Finalize();
    return 0;
}
