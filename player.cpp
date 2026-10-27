#include "game.h"

// プレイヤー
//プレイヤー処理の定義

#pragma region // チュートリアル 関数定義

// チュートリアル
void InitializeTutorialPlayer(Player& player)
{
	player.x = 1 * kMapSize + kMapSize / 2.0f;
	player.y = 18 * kMapSize + kMapSize / 2.0f;

	player.direction = 2;
	player.isMoving = false;
	player.speed = 10.0f;
}

// プレイヤーの移動処理 #チュートリアル
void UpdateTutorialPlayer(
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
	} else if (player.direction == 1)
	{
		nextX += player.speed;
	} else if (player.direction == 2)
	{
		nextY += player.speed;
	} else if (player.direction == 3)
	{
		nextX -= player.speed;
	}

	const float halfSize = kMapSize / 2.0f;

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
				x >= kMaxTutorialWidth ||
				y < 0 ||
				y >= kMaxTutorialHeight
				)
			{
				hitWall = true;
			} else if (mapData1[y][x] == 1)
			{
				hitWall = true;
			} else if (mapData1[y][x] == kBlockUp) {
				if (player.direction != 0) {
					hitWall = true;
				}
			} else if (mapData1[y][x] == kBlockRight) {
				if (player.direction != 1) {
					hitWall = true;
				}
			} else if (mapData1[y][x] == kBlockDown) {
				if (player.direction != 2) {
					hitWall = true;
				}
			} else if (mapData1[y][x] == kBlockLeft) {
				if (player.direction != 3) {
					hitWall = true;
				}
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

#pragma endregion

#pragma region // ステージ1 関数定義

// ステージ1
void InitializeStage1Player(Player& player)
{
	player.x = 1 * kMapSize + kMapSize / 2.0f;
	player.y = 38 * kMapSize + kMapSize / 2.0f;

	player.direction = 2;
	player.isMoving = false;
	player.speed = 10.0f;
}

// プレイヤーの移動処理 #ステージ1
void UpdateStage1Player(
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
	} else if (player.direction == 1)
	{
		nextX += player.speed;
	} else if (player.direction == 2)
	{
		nextY += player.speed;
	} else if (player.direction == 3)
	{
		nextX -= player.speed;
	}

	const float halfSize = kMapSize / 2.0f;

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
				x >= kMaxStage1Width ||
				y < 0 ||
				y >= kMaxStage1Height
				) 
			{
				hitWall = true;
			} else if (mapData2[y][x] == 1)
			{
				hitWall = true;
			} else if (mapData2[y][x] == kBlockUp) {
				if (player.direction != 0) {
					hitWall = true;
				}
			} else if (mapData2[y][x] == kBlockRight) {
				if (player.direction != 1) {
					hitWall = true;
				}
			} else if (mapData2[y][x] == kBlockDown) {
				if (player.direction != 2) {
					hitWall = true;
				}
			} else if (mapData2[y][x] == kBlockLeft) {
				if (player.direction != 3) {
					hitWall = true;
				}
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

#pragma endregion



