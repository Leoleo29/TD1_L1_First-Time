#pragma once

#include <Novice.h>

const int kWindowWidth = 1920;
const int kWindowHeight = 1080;

const int kGameWidth = 800;
const int kGameHeight = 800;

const int kGameX = 560;
const int kGameY = 140;

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