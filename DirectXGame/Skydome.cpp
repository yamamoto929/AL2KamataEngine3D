#include "Skydome.h"

using namespace KamataEngine;

void Skydome::Initialize(KamataEngine::Camera* camera, KamataEngine::Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
};

void Skydome::Update() { 
	worldTransform_.TransferMatrix(); 
};

void Skydome::Draw() {
	model_->Draw(worldTransform_,*camera_);
};