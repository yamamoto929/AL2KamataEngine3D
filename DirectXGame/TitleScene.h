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

public:
	void Initialize();
	void Update();
	void Draw();
};
