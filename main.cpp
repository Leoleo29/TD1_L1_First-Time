#include <Novice.h>

const char kWindowTitle[] = "台パンするキンタロウ・ホズミ";

const int kWindowWidth = 800;
const int kWindowHeight = 800;

const int kMaxWidth = 2280;
const int kMaxLenght = 1520;

enum playerDirection
{
	kUp,
	kDown,
	kRight,
	kLeft
};

struct Vector2
{
	float x;
	float y;
};

struct Player
{
	Vector2 position;
	Vector2 velocity;
	playerDirection direction;
};

struct Camera
{
	Vector2 position;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Player player
	{
		{400.0f, 400.0f}, // position
		{ 0.0f, 0.0f },   // velocity
		kDown             // direction
	};

	Camera camera
	{
		{0.0f, 0.0f}
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

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
			player.velocity.x = 0.0f;
			player.velocity.y = 0.0f;
		}

		// 引っ張られる力
		const float acceleration = 0.2f;

		// 最大速度
		const float maxSpeed = 5.0f;

		// 現在の方向に引っ張る
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

		// 速度を位置に反映
		player.position.x += player.velocity.x;
		player.position.y += player.velocity.y;

		// ワールドの左端
		if (player.position.x < 40.0f)
		{
			player.position.x = 40.0f;
			player.velocity.x = 0.0f;
		}

		// ワールドの右端
		if (player.position.x > kMaxWidth - 40.0f)
		{
			player.position.x = kMaxWidth - 40.0f;
			player.velocity.x = 0.0f;
		}

		// ワールドの上端
		if (player.position.y < 40.0f)
		{
			player.position.y = 40.0f;
			player.velocity.y = 0.0f;
		}

		// ワールドの下端
		if (player.position.y > kMaxLenght - 40.0f)
		{
			player.position.y = kMaxLenght - 40.0f;
			player.velocity.y = 0.0f;
		}

		// カメラをプレイヤーに追従させる
		camera.position.x = player.position.x - kWindowWidth / 2.0f;
		camera.position.y = player.position.y - kWindowHeight / 2.0f;

		// カメラがワールドの外に出ないようにする
		if (camera.position.x < 0.0f)
		{
			camera.position.x = 0.0f;
		}

		if (camera.position.y < 0.0f)
		{
			camera.position.y = 0.0f;
		}

		if (camera.position.x > kMaxWidth - kWindowWidth)
		{
			camera.position.x = kMaxWidth - kWindowWidth;
		}

		if (camera.position.y > kMaxLenght - kWindowHeight)
		{
			camera.position.y = kMaxLenght - kWindowHeight;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// UP
		if (player.direction == kUp)
		{
			Novice::DrawBox(0, 0, kWindowWidth, 20, 0.0f, RED, kFillModeSolid);
		} else { Novice::DrawBox(0, 0, kWindowWidth, 20, 0.0f, WHITE, kFillModeSolid); }

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
		} else {
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
		} else {
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
		} else {
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

		// プレイヤー
		Novice::DrawEllipse(
			static_cast<int>(player.position.x - camera.position.x),
			static_cast<int>(player.position.y - camera.position.y),
			20,
			20,
			0.0f,
			BLUE,
			kFillModeSolid
		);

		Novice::ScreenPrintf(
			10,
			30,
			"Player Position: (%.2f, %.2f)",
			player.position.x,
			player.position.y
		);

		Novice::ScreenPrintf(
			10,
			50,
			"Camera Position: (%.2f, %.2f)",
			camera.position.x,
			camera.position.y
		);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();

	return 0;
}

