#include "TitleScene.h"
#include "WorldMatrixUpdate.h"
using namespace KamataEngine;

void TitleScene::Initialize() { 
	modelPlayer_ = Model::CreateFromOBJ("player", true); 
	modelTitleFont_ = Model::CreateFromOBJ("titleFont", true);

	worldTransformPlayer_.Initialize();

	worldTransformTitleFont_.Initialize();
	

	camera_ = new Camera();
	camera_->Initialize();
};

void TitleScene::Update() { 
	if (Input::GetInstance()->PushKey(DIK_W)) {
		worldTransformPlayer_.translation_.z -= 1.f;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		worldTransformPlayer_.translation_.z += 1.f;
	}

	WorldMatrixUpdate(worldTransformPlayer_);
	WorldMatrixUpdate(worldTransformTitleFont_);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		DebugText::GetInstance()->ConsolePrintf("%f\n", worldTransformPlayer_.translation_.z);
	}
}

void TitleScene::Draw() {
	Model::PreDraw();
	modelPlayer_->Draw(worldTransformPlayer_, *camera_);
	modelTitleFont_->Draw(worldTransformTitleFont_, *camera_);
	Model::PostDraw();
};
