
#pragma once
#include <Novice.h>

enum Scene
{
    kSceneTitle,
    kSceneTutorial,
    kSceneStage1,
    kSceneClear,
    kSceneGameOver
};

const int kWindowWidth = 1920;
const int kWindowHeight = 1080;
const int kGameWidth = 800;
const int kGameHeight = 800;
const int kGameX = (kWindowWidth - kGameWidth) / 2;
const int kGameY = (kWindowHeight - kGameHeight) / 2;

const int kMaxTutorialWidth = 20;
const int kMaxTutorialHeight = 20;
const int kMaxStage1Width = 40;
const int kMaxStage1Height = 40;
const int kMapSize = 80;

const int kBlockUp = 2;
const int kBlockRight = 3;
const int kBlockDown = 4;
const int kBlockLeft = 5;
const int kBlockWarp = 6;
const int kBlockSpike = 7;
const int kBlockKey = 8;
const int kBlockGoal = 9;

const int kRequiredKeyCount = 8;
const int kPlayerMaxHP = 4;
const int kInvincibleTime = 60;

struct Player
{
    float x;
    float y;
    int direction;
    bool isMoving;
    float speed;

    bool warpLocked;
    int warpMapX;
    int warpMapY;

    int hp;
    int invincibleTimer;

    int keyCount;
    bool showKeyArrow;
};

struct Camera
{
    float x;
    float y;
};

extern int mapData1[kMaxTutorialWidth][kMaxTutorialHeight];
extern int mapData2[kMaxStage1Width][kMaxStage1Height];

extern bool keyCollected1[kMaxTutorialHeight][kMaxTutorialWidth];
extern bool keyCollected2[kMaxStage1Height][kMaxStage1Width];

void InitializeTutorialPlayer(Player& player);
void InitializeStage1Player(Player& player);

void UpdateTutorialPlayer(Player& player, char* keys, char* preKeys);
void UpdateStage1Player(Player& player, char* keys, char* preKeys);

void InitializeMap();

bool IsTutorialGoalReached(const Player& player);
bool IsStage1GoalReached(const Player& player);

void UpdateTutorialCamera(Camera& camera, const Player& player);
void UpdateStage1Camera(Camera& camera, const Player& player);

void DrawTutorialMap(
    int blockTexture, int upBlock, int rightBlock,
    int downBlock, int leftBlock, const Camera& camera
);

void DrawStage1Map(
    int blockTexture, int upBlock, int rightBlock,
    int downBlock, int leftBlock, const Camera& camera
);

void DrawBackground(const Player& player, int bar1Texture, int bar2Texture);
void DrawPlayer(const Player& player, const Camera& camera, int playerTexture);
void DrawKeyArrow(const Player& player, const Camera& camera, bool isTutorial);
