#pragma once
#include "KamataEngine.h"
class TitleScene {
private:
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelTitleFont_ = nullptr;

	//KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::WorldTransform worldTransformTitleFont_;
	KamataEngine::Camera* camera_;

	static const inline float kPlayerTranslationY = -2.0f;
	static const inline float kPlayerScale= 1.5f;
	static const inline float kTitleFontScale = 0.7f;
	static const inline float kTitleFontTranslationY = 2.0f;
	static const inline float kCameraTranslationZ = -15.0f;

	static const inline float kTitleFontRotationSpeedY = -0.03f;

	bool finished_ = false;

public:
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }
};
