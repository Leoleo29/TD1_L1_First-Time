#include <Novice.h>
#include <cstring>
#include "game.h"    // ゲームの定義をまとめたヘッダファイル

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


	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player;
	InitializePlayer(player);

	Camera camera = { 0.0f, 0.0f };

	InitializeMap();

	while (Novice::ProcessMessage() == 0)
	{
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		UpdatePlayer(
			player,
			keys,
			preKeys
		);


		UpdateCamera(
			camera,
			player
		);

		// マップデータ
		DrawMap(
			blockTexture,
			camera
		);

		DrawPlayer(
			player,
			camera,
			playerTexture
		);

		DrawBackground(
			player,
			bar1Texture,
			bar2Texture
		);

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