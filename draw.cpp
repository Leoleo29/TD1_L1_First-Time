#include "game.h"

// 描画処理
// 描画処理の定義

#pragma region // チュートリアル 関数定義

// チュートリアルのプレイヤーのカメラ移動
void UpdateTutorialCamera(
	Camera& camera,
	const Player& player
)
{
	// プレイヤーを画面中央に配置する
	camera.x =
		player.x - kGameWidth / 2.0f;

	camera.y =
		player.y - kGameHeight / 2.0f;

	// 左端
	if (camera.x < 0.0f)
	{
		camera.x = 0.0f;
	}

	// 上端
	if (camera.y < 0.0f)
	{
		camera.y = 0.0f;
	}

	// チュートリアルマップの最大カメラ位置
	float maxCameraX =
		kMaxTutorialWidth * kMapSize -
		kGameWidth;

	float maxCameraY =
		kMaxTutorialHeight * kMapSize -
		kGameHeight;

	// 右端
	if (camera.x > maxCameraX)
	{
		camera.x = maxCameraX;
	}

	// 下端
	if (camera.y > maxCameraY)
	{
		camera.y = maxCameraY;
	}
}

// チュートリアルのマップ
void DrawTutorialMap(
	int blockTexture, int upBlock, int rightBlock,
	int downBlock, int leftBlock, const Camera& camera
)
{
	for (int y = 0; y < kMaxTutorialHeight; ++y)
	{
		for (int x = 0; x < kMaxTutorialWidth; ++x)
		{
			const int posX = kGameX + x * kMapSize - static_cast<int>(camera.x);
			const int posY = kGameY + y * kMapSize - static_cast<int>(camera.y);

			if (posX + kMapSize <= kGameX || posX >= kGameX + kGameWidth ||
				posY + kMapSize <= kGameY || posY >= kGameY + kGameHeight)
				continue;

			int texture = 0;
			switch (mapData1[y][x])
			{
			case 1: texture = blockTexture; break;
			case kBlockUp: texture = upBlock; break;
			case kBlockRight: texture = rightBlock; break;
			case kBlockDown: texture = downBlock; break;
			case kBlockLeft: texture = leftBlock; break;
			default: break;
			}

			if (texture != 0)
				Novice::DrawSprite(posX, posY, texture, 1.0f, 1.0f, 0.0f, WHITE);
		}
	}
}

#pragma endregion

#pragma region // ステージ1 関数定義

// ステージ1のプレイヤーのカメラ移動
void UpdateStage1Camera(
	Camera& camera,
	const Player& player
)
{
	camera.x =
		player.x -
		kGameWidth / 2.0f;

	camera.y =
		player.y -
		kGameHeight / 2.0f;


	// 左端
	if (camera.x < 0.0f)
	{
		camera.x = 0.0f;
	}

	// 上端
	if (camera.y < 0.0f)
	{
		camera.y = 0.0f;
	}


	// カメラが移動できる最大値
	float maxStage1CameraX =
		kMaxStage1Width * kMapSize -
		kGameWidth;

	float maxStage1CameraY =
		kMaxStage1Height * kMapSize -
		kGameHeight;


	// 右端
	if (camera.x > maxStage1CameraX)
	{
		camera.x = maxStage1CameraX;
	}

	// 下端
	if (camera.y > maxStage1CameraY)
	{
		camera.y = maxStage1CameraY;
	}
}

// ステージ1のマップ
void DrawStage1Map(
	int blockTexture, int upBlock, int rightBlock,
	int downBlock, int leftBlock, const Camera& camera
)
{
	for (int y = 0; y < kMaxStage1Height; ++y)
	{
		for (int x = 0; x < kMaxStage1Width; ++x)
		{
			const int posX = kGameX + x * kMapSize - static_cast<int>(camera.x);
			const int posY = kGameY + y * kMapSize - static_cast<int>(camera.y);

			if (posX + kMapSize <= kGameX || posX >= kGameX + kGameWidth ||
				posY + kMapSize <= kGameY || posY >= kGameY + kGameHeight)
				continue;

			int texture = 0;
			switch (mapData2[y][x])
			{
			case 1: texture = blockTexture; break;
			case kBlockUp: texture = upBlock; break;
			case kBlockRight: texture = rightBlock; break;
			case kBlockDown: texture = downBlock; break;
			case kBlockLeft: texture = leftBlock; break;
			default: break;
			}

			if (texture != 0)
				Novice::DrawSprite(posX, posY, texture, 1.0f, 1.0f, 0.0f, WHITE);
		}
	}
}

#pragma endregion

// 背景処理
void DrawBackground(
	const Player&player,
	int bar1Texture,
	int bar2Texture
)
{
	// 上
	Novice::DrawBox(
		0,
		0,
		1920,
		140,
		0,
		BLACK,
		kFillModeSolid
	);

	// 左
	Novice::DrawBox(
		0,
		140,
		560,
		800,
		0,
		BLACK,
		kFillModeSolid
	);

	// 右
	Novice::DrawBox(
		1360,
		140,
		560,
		800,
		0,
		BLACK,
		kFillModeSolid
	);

	// 下
	Novice::DrawBox(
		0,
		940,
		1920,
		140,
		0,
		BLACK,
		kFillModeSolid
	);
	
		// 上
		if (player.direction == 0)
		{
			Novice::DrawSprite(
				560,
				100,
				bar1Texture,
				1,
				1,
				0,
				WHITE
			);
		}

		// 右
		if (player.direction == 1)
		{
			Novice::DrawSprite(
				1360,
				140,
				bar2Texture,
				1,
				1,
				0,
				WHITE
			);
		}

		// 下
		if (player.direction == 2)
		{
			Novice::DrawSprite(
				560,
				940,
				bar1Texture,
				1,
				1,
				0,
				WHITE
			);
		}

		// 左
		if (player.direction == 3)
		{
			Novice::DrawSprite(
				520,
				140,
				bar2Texture,
				1,
				1,
				0,
				WHITE
			);
		}
		
}

// プレイヤーの描画処理
void DrawPlayer(
	const Player& player,
	const Camera& camera,
	int playerTexture
)
{
	int drawX =
		kGameX +
		static_cast<int>(
			player.x -
			camera.x -
			kMapSize / 2.0f
			);

	int drawY =
		kGameY +
		static_cast<int>(
			player.y -
			camera.y -
			kMapSize / 2.0f
			);


	Novice::DrawSprite(
		drawX,
		drawY,
		playerTexture,
		1.0f,
		1.0f,
		0.0f,
		WHITE
	);
}

