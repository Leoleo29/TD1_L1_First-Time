#include "game.h"

void InitializePlayer(Player& player)
{
	player.x = 120.0f;
	player.y = 3080.0f;

	// 最初は右
	player.direction = 2;

	// 最初は止まっている
	player.isMoving = false;

	player.speed = 10.0f;
}

void UpdatePlayer(
	Player& player,
	char* keys,
	char* preKeys
)
{
	// SPACEを押した瞬間
	if (
		preKeys[DIK_SPACE] == 0 &&
		keys[DIK_SPACE] != 0
		)
	{
		// 反時計回りに90度
		player.direction--;

		if (player.direction < 0)
		{
			player.direction = 3;
		}

		player.isMoving = true;
	}

	if (!player.isMoving)
	{
		return;
	}

	float nextX = player.x;
	float nextY = player.y;

	if (player.direction == 0)
	{
		nextY -= player.speed;
	}
	else if (player.direction == 1)
	{
		nextX += player.speed;
	}
	else if (player.direction == 2)
	{
		nextY += player.speed;
	}
	else if (player.direction == 3)
	{
		nextX -= player.speed;
	}

	const float halfSize = 40.0f;

	float left = nextX - halfSize;
	float right = nextX + halfSize;
	float top = nextY - halfSize;
	float bottom = nextY + halfSize;

	int leftMapX =
		static_cast<int>(left) / kMapSize;

	int rightMapX =
		static_cast<int>(right - 1.0f) / kMapSize;

	int topMapY =
		static_cast<int>(top) / kMapSize;

	int bottomMapY =
		static_cast<int>(bottom - 1.0f) / kMapSize;

	bool hitWall = false;

	for (int y = topMapY; y <= bottomMapY; y++)
	{
		for (int x = leftMapX; x <= rightMapX; x++)
		{
			if (
				x < 0 ||
				x >= kMaxWidth ||
				y < 0 ||
				y >= kMaxHeight
				)
			{
				hitWall = true;
			}
			else if (mapData[y][x] == 1)
			{
				hitWall = true;
			}
		}
	}

	if (hitWall)
	{
		player.isMoving = false;
		return;
	}

	player.x = nextX;
	player.y = nextY;
}