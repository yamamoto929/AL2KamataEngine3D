#include "TitleScene.h"
using namespace KamataEngine;

void TitleScene::Initialize() { 
	modelPlayer_ = Model::CreateFromOBJ("player", true); 
	modelTitleFont_ = Model::CreateFromOBJ("titleFont", true);

	worldTransform_ .Initialize();
	

	camera_ = new Camera();
	camera_->Initialize();
};

void TitleScene::Update() {

};

void TitleScene::Draw() {
	modelPlayer_->Draw(worldTransform_, *camera_);
	modelTitleFont_->Draw(worldTransform_, *camera_);
};
