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
	worldTransformPlayer_.translation_.y = kPlayerTranslationY;
	worldTransformPlayer_.scale_.x = kPlayerScale;
	worldTransformPlayer_.scale_.y = kPlayerScale;
	worldTransformPlayer_.scale_.z = kPlayerScale;

	worldTransformTitleFont_.Initialize();
	worldTransformTitleFont_.scale_.x = kTitleFontScale;
	worldTransformTitleFont_.scale_.y = kTitleFontScale;
	worldTransformTitleFont_.scale_.z = kTitleFontScale;
	
	worldTransformTitleFont_.translation_.y = kTitleFontTranslationY;

	camera_ = new Camera();
	camera_->Initialize();

	camera_->translation_.z = kCameraTranslationZ;
};

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	worldTransformTitleFont_.rotation_.y += kTitleFontRotationSpeedY;
	
	WorldMatrixUpdate(worldTransformPlayer_);
	WorldMatrixUpdate(worldTransformTitleFont_);

	camera_->UpdateMatrix();
}

void TitleScene::Draw() {
	Model::PreDraw();
	modelPlayer_->Draw(worldTransformPlayer_, *camera_);
	modelTitleFont_->Draw(worldTransformTitleFont_, *camera_);
	Model::PostDraw();
};
