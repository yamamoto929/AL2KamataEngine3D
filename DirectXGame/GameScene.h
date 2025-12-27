#pragma once
#include "KamataEngine.h"

// ゲームシーン
class GameScene {
private:
	// KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;

	uint32_t textureHandle_ = 0;

	uint32_t soundDataHandle_ = 0;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	uint32_t voiceHandle_ = 0;

	uint32_t playHandle_ = 0;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;

public:
	~GameScene();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};
