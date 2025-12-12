#pragma once
#include "KamataEngine.h"

// ゲームシーン
class GameScene {
private:
	uint32_t textureHandle_ = 0;

public:
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();
};
