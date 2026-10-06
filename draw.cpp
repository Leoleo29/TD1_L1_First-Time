#include "game.h"

void DrawMap(
	int blockGraphHandle,
	const Camera& camera
)
{
	// 背景
	Novice::DrawBox(
		0,
		0,
		kWindowWidth,
		kWindowHeight,
		0.0f,
		BLACK,
		kFillModeSolid
	);

	// マップ
	for (int y = 0; y < kMaxHeight; y++)
	{
		for (int x = 0; x < kMaxWidth; x++)
		{
			if (mapData[y][x] == 1)
			{
				int posX =
					x * kMapSize -
					static_cast<int>(camera.position.x);

				int posY =
					y * kMapSize -
					static_cast<int>(camera.position.y);

				Novice::DrawSprite(
					posX,
					posY,
					blockGraphHandle,
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
	const Camera& camera
)
{
	int drawX =
		static_cast<int>(
			player.position.x - camera.position.x
			);

	int drawY =
		static_cast<int>(
			player.position.y - camera.position.y
			);

	Novice::DrawEllipse(
		drawX,
		drawY,
		20,
		20,
		0.0f,
		BLUE,
		kFillModeSolid
	);
}

void DrawDirectionBorder(
	const Player& player
)
{
	// UP
	if (player.direction == kUp)
	{
		Novice::DrawBox(
			0, 0,
			kWindowWidth, 20,
			0.0f,
			RED,
			kFillModeSolid
		);
	} else
	{
		Novice::DrawBox(
			0, 0,
			kWindowWidth, 20,
			0.0f,
			WHITE,
			kFillModeSolid
		);
	}

	// DOWN
	if (player.direction == kDown)
	{
		Novice::DrawBox(
			0,
			kWindowHeight - 20,
			kWindowWidth,
			20,
			0.0f,
			RED,
			kFillModeSolid
		);
	} else
	{
		Novice::DrawBox(
			0,
			kWindowHeight - 20,
			kWindowWidth,
			20,
			0.0f,
			WHITE,
			kFillModeSolid
		);
	}

	// RIGHT
	if (player.direction == kRight)
	{
		Novice::DrawBox(
			kWindowWidth - 20,
			0,
			20,
			kWindowHeight,
			0.0f,
			RED,
			kFillModeSolid
		);
	} else
	{
		Novice::DrawBox(
			kWindowWidth - 20,
			0,
			20,
			kWindowHeight,
			0.0f,
			WHITE,
			kFillModeSolid
		);
	}

	// LEFT
	if (player.direction == kLeft)
	{
		Novice::DrawBox(
			0,
			0,
			20,
			kWindowHeight,
			0.0f,
			RED,
			kFillModeSolid
		);
	} else
	{
		Novice::DrawBox(
			0,
			0,
			20,
			kWindowHeight,
			0.0f,
			WHITE,
			kFillModeSolid
		);
	}
}
