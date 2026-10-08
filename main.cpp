#include <Novice.h>
#include <cstring>
#include "game.h"    // ゲームの宣言をまとめたヘッダファイル

const char kWindowTitle[] =
"台パンするキンタロウ・ホズミ  ゼン";

int WINAPI WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	Novice::Initialize(
		kWindowTitle,
		1920,
		1080
	);

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
	InitializePlayer(player);

	Camera camera = { 0.0f, 0.0f };

	InitializeMap();

	Scene scene = kSceneTitle;

	while (Novice::ProcessMessage() == 0)
	{
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

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
				scene = kSceneGame;
			}

			break;

		case kSceneGame:

			// プレイヤー更新
			UpdatePlayer(
				player,
				keys,
				preKeys
			);

			// カメラ更新
			UpdateCamera(
				camera,
				player
			);

			// マップ描画
			DrawMap(
				blockTexture,
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
			break;
		}

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