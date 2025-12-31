#pragma once
#include "KamataEngine.h"
#include "Player.h"

// ゲームシーン
class GameScene {
private:
	uint32_t textureHandle_ = 0;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform* worldTransform_;
	KamataEngine::Camera* camera_;

	Player* player_ = nullptr;

public:
	~GameScene();
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};
