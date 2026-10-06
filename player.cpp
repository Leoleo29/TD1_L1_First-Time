#include "game.h"

void InitializePlayer(Player& player)
{
	// 上のコードを基準に初期位置を設定
	player.position = { 400.0f, 400.0f };
	player.velocity = { 0.0f, 0.0f };
	player.direction = kDown;
}

void UpdatePlayer(
	Player& player,
	char* keys,
	char* preKeys
)
{
	// SPACEを押した瞬間に方向を切り替える
	if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0)
	{
		switch (player.direction)
		{
		case kDown:
			player.direction = kRight;
			break;

		case kRight:
			player.direction = kUp;
			break;

		case kUp:
			player.direction = kLeft;
			break;

		case kLeft:
			player.direction = kDown;
			break;
		}

		// 向きを変えたら速度をリセット
		player.velocity = { 0.0f, 0.0f };
	}

	// 引っ張られる力
	const float acceleration = 0.2f;

	// 最大速度
	const float maxSpeed = 5.0f;

	// 現在の方向に加速
	switch (player.direction)
	{
	case kUp:
		player.velocity.y -= acceleration;

		if (player.velocity.y < -maxSpeed)
		{
			player.velocity.y = -maxSpeed;
		}
		break;

	case kDown:
		player.velocity.y += acceleration;

		if (player.velocity.y > maxSpeed)
		{
			player.velocity.y = maxSpeed;
		}
		break;

	case kRight:
		player.velocity.x += acceleration;

		if (player.velocity.x > maxSpeed)
		{
			player.velocity.x = maxSpeed;
		}
		break;

	case kLeft:
		player.velocity.x -= acceleration;

		if (player.velocity.x < -maxSpeed)
		{
			player.velocity.x = -maxSpeed;
		}
		break;
	}

	// 次の位置
	float nextX = player.position.x + player.velocity.x;
	float nextY = player.position.y + player.velocity.y;

	// プレイヤーが入る予定のマス
	int currentMapX =
		static_cast<int>(player.position.x) / kMapSize;

	int currentMapY =
		static_cast<int>(player.position.y) / kMapSize;

	int nextMapX =
		static_cast<int>(nextX) / kMapSize;

	int nextMapY =
		static_cast<int>(nextY) / kMapSize;

	// 別のマスへ入る場合だけ壁をチェック
	if (currentMapX != nextMapX || currentMapY != nextMapY)
	{
		if (
			nextMapX < 0 ||
			nextMapX >= kMaxWidth ||
			nextMapY < 0 ||
			nextMapY >= kMaxHeight ||
			mapData[nextMapY][nextMapX] == 1
			)
		{
			// 壁にぶつかったらその方向の速度を止める
			if (player.direction == kUp || player.direction == kDown)
			{
				player.velocity.y = 0.0f;
			} else
			{
				player.velocity.x = 0.0f;
			}

			return;
		}
	}

	// ワールドの端
	const float minPosition = 40.0f;
	const float maxPositionX = kMaxWidth * kMapSize - 40.0f;
	const float maxPositionY = kMaxHeight * kMapSize - 40.0f;

	if (nextX < minPosition)
	{
		nextX = minPosition;
		player.velocity.x = 0.0f;
	}

	if (nextX > maxPositionX)
	{
		nextX = maxPositionX;
		player.velocity.x = 0.0f;
	}

	if (nextY < minPosition)
	{
		nextY = minPosition;
		player.velocity.y = 0.0f;
	}

	if (nextY > maxPositionY)
	{
		nextY = maxPositionY;
		player.velocity.y = 0.0f;
	}

	player.position.x = nextX;
	player.position.y = nextY;
}
