#pragma once

#include <Novice.h>

// ==============================
// 定数
// ==============================
const int kWindowWidth = 800;
const int kWindowHeight = 800;

const int kMaxWidth = 40;
const int kMaxHeight = 40;
const int kMapSize = 80;

// ==============================
// プレイヤーの方向
// ==============================
enum PlayerDirection
{
	kUp,
	kDown,
	kRight,
	kLeft
};

// ==============================
// Vector2
// ==============================
struct Vector2
{
	float x;
	float y;
};

// ==============================
// Player
// ==============================
struct Player
{
	Vector2 position;
	Vector2 velocity;
	PlayerDirection direction;
};

// ==============================
// Camera
// ==============================
struct Camera
{
	Vector2 position;
};

// ==============================
// Map
// ==============================
extern int mapData[kMaxHeight][kMaxWidth];

// ==============================
// Player
// ==============================
void InitializePlayer(Player& player);

void UpdatePlayer(
	Player& player,
	char* keys,
	char* preKeys
);

// ==============================
// Map
// ==============================
void InitializeMap();

// ==============================
// Camera / Game
// ==============================
void UpdateCamera(
	Camera& camera,
	const Player& player
);

// ==============================
// Draw
// ==============================
void DrawMap(
	int blockGraphHandle,
	const Camera& camera
);

void DrawPlayer(
	const Player& player,
	const Camera& camera
);

void DrawDirectionBorder(
	const Player& player
);

// ==============================
// Game
// ==============================
void InitializeGame(
	Player& player,
	Camera& camera
);

void UpdateGame(
	Player& player,
	Camera& camera,
	char* keys,
	char* preKeys
);

void DrawGame(
	const Player& player,
	const Camera& camera,
	int blockGraphHandle
);
