#include "Player.h"
#include <cassert>
#include "AffineMatrix.h"


using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) { 
	assert(model); 
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	camera_ = camera;
}

void Player::Update() { 
	Matrix4x4 affineMatrix = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = affineMatrix;
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_,*camera_,textureHandle_);
}