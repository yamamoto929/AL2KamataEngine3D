#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "CameraController.h"
#include "DeathParticles.h"

// ゲームシーン
class GameScene {
private:
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleBlock_ = 0;

	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::WorldTransform* worldTransform_;
	KamataEngine::Camera* camera_;

	Player* player_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	bool isDebugCameraActive_ = false;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	MapChipField* mapChipField_ = nullptr;

	CameraController* cameraController_ = nullptr;

	const uint32_t kEnemyMax = 1; 
	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* modelDeathParticles_ = nullptr;

public:
	~GameScene();
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	void GenerateBlocks();

	void CheckAllCollisions();
};
