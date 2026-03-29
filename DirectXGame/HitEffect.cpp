#include "HitEffect.h"
#include "Easing.h"
#include "RandomNumberGenerator.h"
#include "WorldMatrixUpdate.h"
#include <cassert>
#include <numbers>
using namespace KamataEngine;
Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

void HitEffect::Initialize(Vector3 spawnPoint) {
	circleWorldTransform_.Initialize();
	circleWorldTransform_.translation_ = spawnPoint;

	circleColor_.Initialize();
	circleColorAlpha_ = 1.0f;

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.scale_ = {0.1f, 1.6f, 1.0f};
		float rand = RNG::GetInstance().Generate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
		worldTransform.rotation_ = {0.0f, 0.0f, rand};
		worldTransform.translation_ = spawnPoint;

		worldTransform.Initialize();
	}
	animCount_ = 0.0f;
	state_ = State::kSpread;
};

void HitEffect::Update() {
	animCount_ += 1.0f / 60.0f;
	switch (state_) {
	case State::kUnknown:
	default: {

		break;
	}
	case State::kSpread: {
		float t = animCount_ / kSpreadTime;
		circleWorldTransform_.scale_.z = EaseOut(0.5f, 1.2f, t);
		circleWorldTransform_.scale_.y = EaseOut(0.5f, 1.2f, t);
		circleWorldTransform_.scale_.x = EaseOut(0.5f, 1.2f, t);

		// フェードアウト移行
		if (animCount_ >= kSpreadTime) {
			state_ = State::kFade;
			animCount_ = 0.0f;
		}
		break;
	}
	case State::kFade: {
		float t = animCount_ / kFadeTime;
		circleColorAlpha_ = EaseOut(1.0f, 0.2f, t);

		// デス移行
		if (animCount_ >= kFadeTime) {
			state_ = State::kDeath;
			animCount_ = 0.0f;
		}
		break;
	}
	case State::kDeath: {
		break;
	}
	}

	WorldMatrixUpdate(circleWorldTransform_);
	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		WorldMatrixUpdate(worldTransform);
	}
};

void HitEffect::Draw() {
	if (camera_ == nullptr) {
		return;
	}

	if (state_ == State::kDeath) {
		return;
	}

	if (model_) {

		circleColor_.SetColor(Vector4{1.0f, 1.0f, 1.0f, circleColorAlpha_});
		model_->Draw(circleWorldTransform_, *camera_, &circleColor_);
	}

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		model_->Draw(worldTransform, *camera_);
	}
}

HitEffect* HitEffect::Create(Vector3 spawnPoint) {
	HitEffect* instance = new HitEffect();
	assert(instance);
	instance->Initialize(spawnPoint);
	return instance;
};
