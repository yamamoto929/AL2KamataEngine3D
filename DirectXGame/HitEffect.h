#pragma once
#include "KamataEngine.h"
class HitEffect {
private:
	enum class State {
		kUnknown,
		kSpread,
		kFade,
		kDeath
	};

	static inline const size_t kEllipseNum = 2;
	static KamataEngine::Model* model_;
	static KamataEngine::Camera* camera_;
	KamataEngine::WorldTransform circleWorldTransform_;
	std::array<KamataEngine::WorldTransform, kEllipseNum> ellipseWorldTransforms_;
	static inline const float kSpreadTime = 1.0f;
	static inline const float kFadeTime = 0.5f;
	State state_ = State::kUnknown;
	float animCount_ = 0.0f;
	float circleColorAlpha_ = 0.0f;
	KamataEngine::ObjectColor circleColor_;

public:
	void Initialize(KamataEngine::Vector3 spawnPoint);
	void Update();
	void Draw();
	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }
	static HitEffect* Create(KamataEngine::Vector3 spawnPoint);
	bool IsDead() const { return state_ == State::kDeath; }
};
