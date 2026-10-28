
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

	// 画像の読み込み
	// 壁のデータ
	int blockTexture = Novice::LoadTexture("./images/block.png");
	// プレイヤーのデータ
	// プレイヤーのデータを変える事!!
	// 旧
	int playerTexture = Novice::LoadTexture("./images/player.png");
	// 新
	//int player1=
	//	Novice::LoadTexture("./player1.png");  // 下
	//int player2 =
	//	Novice::LoadTexture("./player2.png");  // 上
	//int player3 =
	//	Novice::LoadTexture("./player3.png");  // 右
	//int player4 =
	//	Novice::LoadTexture("./player4.png");  // 下

	// 進行方向　上下のデータ
	int bar1Texture = Novice::LoadTexture("./images/bar1.png");
	// 進行方向　左右のデータ
	int bar2Texture = Novice::LoadTexture("./images/bar2.png");

	// 一方通行 のデータ
	int upBlock = Novice::LoadTexture("./images/oneWay_up.png");     // 上
	int rightBlock = Novice::LoadTexture("./images/oneWay_right.png");  // 右
	int downBlock = Novice::LoadTexture("./images/oneWay_down.png");   // 下
	int leftBlock = Novice::LoadTexture("./images/oneWay_left.png");   // 左

	// シーン画像
	// タイトル
	int titleTexture = Novice::LoadTexture("./images/SCENE_Title.png");
	// クリア
	int clearTexture = Novice::LoadTexture("./images/SCENE_Clear.png");
	// ゲームオーバー
	int gameOverTexture = Novice::LoadTexture("./images/SCENE_GameOver.png");

	// ワープホール
	int warp1 =
		Novice::LoadTexture("./images/warp1.png");  // 1
	//int warp2 =
	//	Novice::LoadTexture("./warp2.png");  // 2
	//int warp3 =
	//	Novice::LoadTexture("./warp3.png");  // 3
	//int warp4 =
	//	Novice::LoadTexture("./warp4.png");  // 4

	// ゴール のデータ
	/*int goal =
		Novice::LoadTexture("./goal.png");*/

		// トラップ
	int spikeTrap =
		Novice::LoadTexture("./images/spikeTrap.png");

	// ハート(体力回復)
	/*int heart =
			   Novice::LoadTexture("./heart.png");*/

			   // 宝石
			   //int jewelry1 =
			   //	Novice::LoadTexture("./jewelry1.png");  // 1 (赤)
			   //int jewelry2 =
			   //	Novice::LoadTexture("./jewelry2.png");  // 2 (青)
			   //int jewelry3 =
			   //	Novice::LoadTexture("./jewelry3.png");  // 3 (緑)
			   //int jewelry4 =
						  //	Novice::LoadTexture("./jewelry4.png");  // 4 (黄色)

						  // UI
						  // プレイヤーの体力
						  //int heart_UI_1 =
						  //	Novice::LoadTexture("./heart_UI_1.png");  // ハート x 4
						  //int heart_UI_2 =
						  //	Novice::LoadTexture("./heart_UI_2.png");  // ハート x 3
						  //int heart_UI_3 =
						  //	Novice::LoadTexture("./heart_UI_3.png");  // ハート x 2
						  //int heart_UI_4 =
						  //	Novice::LoadTexture("./heart_UI_4.png");  // ハート x 1

	// ハンドルの取得
	// タイトル
	const int titleBGMHandle =
		Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-173.mp3");
	// プレイ画面 (チュートリアル)
	const int kMokugyoHandle =
		Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-155.mp3");
	//  プレイ画面 (ステージ1)
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-112.mp3");
	// クリア画面
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-156.mp3");
	// ゲームオーバー画面
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/MusMus-BGM-110.mp3");
	// トラップ 
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/斬撃1.mp3");
	// ワープ
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/おばけの笑い声2.mp3");
	// プレイヤー 足音
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/足音1.mp3");
	// 鍵ゲット
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/ジャジャーン2.mp3");
	// ゴール
	const int kStartHandle =
		Novice::LoadAudio("./Resources/Sounds/マリンバグリッサンド3.mp3");

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
				scene = kSceneStage1;
				break;
			}

			UpdateTutorialCamera(camera, player);

			DrawTutorialMap(
				blockTexture,
				upBlock,
				rightBlock,
				downBlock,
				leftBlock,
				warp1,
				spikeTrap,
				camera
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
				blockTexture,
				upBlock,
				rightBlock,
				downBlock,
				leftBlock,
				warp1,
				spikeTrap,
				camera
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
