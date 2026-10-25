#pragma once
#include <Novice.h>

// 宣言

// シーン処理
enum Scene
{
	kSceneTitle,
	kSceneTutorial,
	kSceneStage1,
	kSceneClear,
	kSceneGameOver
};

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

const int kMaxTutorialWidth = 20;
const int kMaxTutorialHeight = 20;

const int kMaxStage1Width = 40;
const int kMaxStage1Height = 40;

const int kMapSize = 80;

const int kBlockUp = 2;
const int kBlockRight = 3;
const int kBlockDown = 4;
const int kBlockLeft = 5;

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

extern int mapData1[kMaxTutorialWidth][kMaxTutorialHeight];

extern int mapData2[kMaxStage1Width][kMaxStage1Height];

void InitializePlayer(Player& player);

void UpdatePlayer(
	Player& player,
	char* keys,
	char* preKeys
);

void InitializeMap();

void UpdateTutorialCamera(
	Camera& camera,
	const Player& player
);

void UpdateStage1Camera(
	Camera& camera,
	const Player& player
);

void DrawBackground(
	const Player& player,
	int bar1Texture,
	int bar2Texture
);

void DrawTutorialMap(
	int blockTexture,
	int upBlock,
	int rightBlock,
	int downBlock,
	int leftBlock,
	const Camera& camera
);

void DrawStage1Map(
	int blockTexture,
	const Camera& camera
);


void DrawPlayer(
	const Player& player,
	const Camera& camera,
	int playerTexture
);
