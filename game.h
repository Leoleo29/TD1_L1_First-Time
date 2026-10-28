#pragma once
#include <Novice.h>

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

const int kGameWidth = 800;
const int kGameHeight = 800;

const int kGameX = (kWindowWidth - kGameWidth) / 2;
const int kGameY = (kWindowHeight - kGameHeight) / 2;

const float kGameCenterX = kGameWidth / 2.0f;
const float kGameCenterY = kGameHeight / 2.0f;

// マップサイズ
const int kMaxTutorialWidth = 20;
const int kMaxTutorialHeight = 20;

const int kMaxStage1Width = 40;
const int kMaxStage1Height = 40;

const int kMapSize = 80;

// マップチップ
const int kBlockUp = 2;
const int kBlockRight = 3;
const int kBlockDown = 4;
const int kBlockLeft = 5;
const int kBlockWarp = 6;
const int kBlockSpike = 7;

// HP設定
const int kPlayerMaxHP = 3;

// 無敵時間（60フレーム）
const int kInvincibleTime = 60;

// プレイヤー
struct Player
{
    float x;
    float y;

    int direction;
    bool isMoving;
    float speed;

    // ワープ直後の連続ワープ防止
    bool warpLocked;
    int warpMapX;
    int warpMapY;

    // HP管理
    int hp;

    // ダメージを受けた後の無敵時間
    int invincibleTimer;
};

// カメラ
struct Camera
{
    float x;
    float y;
};

// マップデータ
extern int mapData1[kMaxTutorialWidth][kMaxTutorialHeight];
extern int mapData2[kMaxStage1Width][kMaxStage1Height];

// プレイヤー初期化
void InitializeTutorialPlayer(Player& player);
void InitializeStage1Player(Player& player);

// プレイヤー更新
void UpdateTutorialPlayer(Player& player, char* keys, char* preKeys);
void UpdateStage1Player(Player& player, char* keys, char* preKeys);

void InitializeMap();

// カメラ
void UpdateTutorialCamera(Camera& camera, const Player& player);
void UpdateStage1Camera(Camera& camera, const Player& player);

// マップ描画
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
    int upBlock,
    int rightBlock,
    int downBlock,
    int leftBlock,
    const Camera& camera
);

// 背景
void DrawBackground(
    const Player& player,
    int bar1Texture,
    int bar2Texture
);

// プレイヤー描画
void DrawPlayer(
    const Player& player,
    const Camera& camera,
    int playerTexture
);