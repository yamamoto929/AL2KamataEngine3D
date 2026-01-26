#include "Enemy.h"
#include "Vector3Operator.h"
#include "WorldMatrixUpdate.h"
#include <cassert>
#include <numbers>
using namespace KamataEngine;
void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	camera_ = camera;
	velocity_ = Vector3{-kMoveSpeed, 0.0f, 0.0f};
	walkTimer_ = 0.0f;
};

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	walkTimer_ += 1.0f / 60.0f;
	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = degree * std::numbers::pi_v<float> / 180.0f;
	WorldMatrixUpdate(worldTransform_);
};

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); };