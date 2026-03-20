#include "TitleScene.h"
#include "WorldMatrixUpdate.h"
#include <numbers>
using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete modelPlayer_;
	delete modelTitleFont_;
	delete camera_;
};

void TitleScene::Initialize() { 
	modelPlayer_ = Model::CreateFromOBJ("player", true); 
	modelTitleFont_ = Model::CreateFromOBJ("titleFont", true);

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
	worldTransformTitleFont_.translation_.y -= 2.0f;
	worldTransformPlayer_.scale_.x *= 1.5f;
	worldTransformPlayer_.scale_.y *= 1.5f;
	worldTransformPlayer_.scale_.z *= 1.5f;

	worldTransformTitleFont_.Initialize();
	worldTransformTitleFont_.scale_.x *= 0.7f;
	worldTransformTitleFont_.scale_.y *= 0.7f;
	worldTransformTitleFont_.scale_.z *= 0.7f;
	
	worldTransformTitleFont_.translation_.y += 2.0f;

	camera_ = new Camera();
	camera_->Initialize();

	camera_->translation_.z = -15.0f;
};

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_W)) {
		worldTransformPlayer_.translation_.z -= 1.f;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		worldTransformPlayer_.translation_.z += 1.f;
	}

	WorldMatrixUpdate(worldTransformPlayer_);
	WorldMatrixUpdate(worldTransformTitleFont_);

	camera_->UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		DebugText::GetInstance()->ConsolePrintf("%f\n", camera_->translation_.z);
	}
}

void TitleScene::Draw() {
	Model::PreDraw();
	modelPlayer_->Draw(worldTransformPlayer_, *camera_);
	modelTitleFont_->Draw(worldTransformTitleFont_, *camera_);
	Model::PostDraw();
};
