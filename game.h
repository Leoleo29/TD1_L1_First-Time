#pragma once
#include <Novice.h>

// 宣言

// ウィンドウサイズ
const int kWindowWidth = 1920;
const int kWindowHeight = 1080;
// プレイ画面サイズ
const int kGameWidth = 800;
const int kGameHeight = 800;
// プレイ画面の左上
const int kGameX =
(kWindowWidth - kGameWidth) / 2;

const int kGameY =
(kWindowHeight - kGameHeight) / 2;

// プレイ画面の中央
const float kGameCenterX =
kGameWidth / 2.0f;

const float kGameCenterY =
kGameHeight / 2.0f;

const int kMaxWidth = 40;
const int kMaxHeight = 40;
const int kMapSize = 80;

struct Player
{
	float x;
	float y;

	int direction;

	bool isMoving;

	float speed;
};

struct Camera
{
	float x;
	float y;
};

extern int mapData[kMaxHeight][kMaxWidth];

void InitializePlayer(Player& player);

void UpdatePlayer(
	Player& player,
	char* keys,
	char* preKeys
);

void InitializeMap();

void UpdateCamera(
	Camera& camera,
	const Player& player
);

void DrawBackground(
	const Player& player,
	int bar1Texture,
	int bar2Texture
);

void DrawMap(
	int blockTexture,
	const Camera& camera
);

void DrawPlayer(
	const Player& player,
	const Camera& camera,
	int playerTexture
);

// シーン処理
enum Scene
{
	kSceneTitle,
	kSceneGame,
	kSceneClear,
	kSceneGameOver
};