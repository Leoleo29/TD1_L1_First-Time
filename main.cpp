#include <Novice.h>
#include "game.h"

const char kWindowTitle[] =
"台パンするキンタロウ・ホズミ  ゼン";

int WINAPI WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	// Novice初期化
	Novice::Initialize(
		kWindowTitle,
		kWindowWidth,
		kWindowHeight
	);

	// ブロック画像
	int blockGraphHandle =
		Novice::LoadTexture("./images/block.png");

	// ゲームで使用するデータ
	Player player{};
	Camera camera{};

	InitializeGame(player, camera);

	// キー入力
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// メインループ
	while (Novice::ProcessMessage() == 0)
	{
		Novice::BeginFrame();

		// キー入力
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// ==============================
		// 更新
		// ==============================
		UpdateGame(
			player,
			camera,
			keys,
			preKeys
		);

		// ==============================
		// 描画
		// ==============================
		DrawGame(
			player,
			camera,
			blockGraphHandle
		);

		Novice::EndFrame();

		// ESCで終了
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
