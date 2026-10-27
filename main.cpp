#include <Novice.h>
#include <Windows.h>
#include <cstring>
#include "game.h"    // ゲームの宣言をまとめたヘッダファイル
#include "fullScreen.h"

const char kWindowTitle[] =
"台パンするキンタロウ・ホズミ  ゼン";

int WINAPI WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	// ウィンドウの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	
	// Noviceのウィンドウハンドルを取得
	HWND gameWindow = GetActiveWindow();

	// ウィンドウの位置・サイズを保存する変数
	RECT windowRect = {};

	// ウィンドウのスタイルを保存
	LONG_PTR windowStyle = 0;

	// フルスクリーン状態
	bool isFullScreen = false;

	if (gameWindow == nullptr)
	{
		OutputDebugStringA(
			"gameWindow is nullptr\n"
		);
	}
#pragma region // タイトル
	// 矢印
	/*int title_arrowTexture = 
		Novice::title_arrowTexture("./images/UnderConstruction/title_arrow.png");*/



#pragma endregion

	// 画像の読み込み
	// 壁のデータ
	int blockTexture =
		Novice::LoadTexture("./images/block.png");
	// プレイヤーのデータ
	int playerTexture =
		Novice::LoadTexture("./images/player.png");
	// 進行方向　上下のデータ
	int bar1Texture =
		Novice::LoadTexture("./images/bar1.png");
	// 進行方向　左右のデータ
	int bar2Texture =
		Novice::LoadTexture("./images/bar2.png");
	int upBlock =
		Novice::LoadTexture("./images/up.png");
	int rightBlock =
		Novice::LoadTexture("./images/right.png");
	int downBlock =
		Novice::LoadTexture("./images/down.png");
	int leftBlock =
		Novice::LoadTexture("./images/left.png");
	// シーン画像
	// タイトル
	int SCENE_TitleTexture =
		Novice::LoadTexture("./images/SCENE_Title.png");
	// クリア
	int SCENE_ClearTexture =
		Novice::LoadTexture("./images/SCENE_Clear.png");
	// ゲームオーバー
	int SCENE_GameOverTexture =
		Novice::LoadTexture("./images/SCENE_GameOver.png");


	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player;
	InitializeTutorialPlayer(player);

	Camera camera = { 0.0f, 0.0f };

	InitializeMap();

	Scene scene = kSceneTitle;

	while (Novice::ProcessMessage() == 0)
	{
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// F11キーが押された瞬間に処理する
		if (
			keys[DIK_F11] != 0 &&
			preKeys[DIK_F11] == 0
			)
		{
			// F11が押されたか確認する
			OutputDebugStringA("F11 pressed\n");

			// フルスクリーンとウィンドウ表示を切り替える
			ToggleFullScreen(
				gameWindow,
				windowRect,
				windowStyle,
				isFullScreen
			);
		}

		switch (scene)
		{
		case kSceneTitle:

			// タイトル画面の更新・描画
			Novice::DrawSprite(
				0,
				0,
				SCENE_TitleTexture,
				1.0f,
				1.0f,
				0.0f,
				WHITE
			);
			// 〇〇が押されたらゲームスタート
			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0)
			{
				scene = kSceneTutorial;
			}

			// debug用 チュートリアル
			if (preKeys[DIK_1] == 0 && keys[DIK_1] != 0)
			{
				InitializeTutorialPlayer(player);
				camera = { 0.0f, 0.0f };
				scene = kSceneTutorial;
			}

			// debug用 ステージ1
			if (preKeys[DIK_2] == 0 && keys[DIK_2] != 0)
			{
				InitializeStage1Player(player);
				camera = { 0.0f, 0.0f };
				scene = kSceneStage1;
			}

			break;

		case kSceneTutorial:

			// プレイヤー更新
			UpdateTutorialPlayer(
				player,
				keys,
				preKeys
			);

			// カメラ更新
			UpdateTutorialCamera(
				camera,
				player
			);

			// マップ描画
			DrawTutorialMap(
				blockTexture,
				upBlock,
				rightBlock,
				downBlock,
				leftBlock,
				camera
			);

			// プレイヤー描画
			DrawPlayer(
				player,
				camera,
				playerTexture
			);

			// 背景描画
			DrawBackground(
				player,
				bar1Texture,
				bar2Texture
			);

			break;

		case kSceneStage1:

			// プレイヤー更新
			UpdateStage1Player(
				player,
				keys,
				preKeys
			);

			// カメラ更新
			UpdateStage1Camera(
				camera,
				player
			);

			// マップ描画
			DrawStage1Map(
				blockTexture,
				upBlock,
				rightBlock,
				downBlock,
				leftBlock,
				camera
			);

			// プレイヤー描画
			DrawPlayer(
				player,
				camera,
				playerTexture
			);

			// 背景描画
			DrawBackground(
				player,
				bar1Texture,
				bar2Texture
			);

			break;

		case kSceneClear:

			// クリア画面
			Novice::DrawSprite(
				0,
				0,
				SCENE_ClearTexture,
				1.0f,
				1.0f,
				0.0f,
				WHITE
			);

			// 〇〇が押されたらゲームスタート
			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{
				scene = kSceneTitle;
			}

			break;

		case kSceneGameOver:

			// ゲームオーバー画面
			Novice::DrawSprite(
				0,
				0,
				SCENE_GameOverTexture,
				1.0f,
				1.0f,
				0.0f,
				WHITE
			);

			// 〇〇が押されたらゲームスタート
			if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
			{
				scene = kSceneTitle;
			}

			break;

		}

		Novice::ScreenPrintf(
			10, 10,
			"full screen : %d",
			isFullScreen ? 1 : 0
		);

		Novice::ScreenPrintf(
			10, 30,
			"window handle : %d",
			gameWindow != nullptr ? 1 : 0
		);

		/*Novice::ScreenPrintf(
			10, 40,
			"scene : %c",
			Scene
		);*/

		Novice::EndFrame();

		if (
			preKeys[DIK_ESCAPE] == 0 &&
			keys[DIK_ESCAPE] != 0
			)
		{
			break;
		}
	}

	Novice::Finalize();

	return 0;
}