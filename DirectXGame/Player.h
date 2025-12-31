#pragma once
#include "KamataEngine.h"

class Player {
private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle"> テクスチャハンドル</param>
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle,KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
};