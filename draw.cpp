#include "game.h"

// 描画処理
// 描画処理の定義

void UpdateCamera(
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
	float maxCameraX =
		kMaxWidth * kMapSize -
		kGameWidth;

	float maxCameraY =
		kMaxHeight * kMapSize -
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

void DrawMap(
	int blockTexture,
	const Camera& camera
)
{
	for (int i = 0; i < kMaxHeight; i++)
	{
		for (int j = 0; j < kMaxWidth; j++)
		{
			if (mapData[i][j] == 1)
			{
				int posX =
					kGameX +
					j * kMapSize -
					static_cast<int>(camera.x);

				int posY =
					kGameY +
					i * kMapSize -
					static_cast<int>(camera.y);


				// プレイ画面外なら描画しない
				if (
					posX + kMapSize <= kGameX ||
					posX >= kGameX + kGameWidth ||
					posY + kMapSize <= kGameY ||
					posY >= kGameY + kGameHeight
					)
				{
					continue;
				}


				Novice::DrawSprite(
					posX,
					posY,
					blockTexture,
					1.0f,
					1.0f,
					0.0f,
					WHITE
				);
			}
		}
	}
}

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

