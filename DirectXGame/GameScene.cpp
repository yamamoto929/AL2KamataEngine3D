#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() { 
	delete model_; 
}

// 初期化
void GameScene::Initialize() { 
	textureHandle_ = TextureManager::Load("uvChecker.png"); 
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
};

// 更新処理
void GameScene::Update() {

};

// 描画処理
void GameScene::Draw() { 
	Model::PreDraw();
	model_->Draw(worldTransform_, camera_, textureHandle_);
	Model::PostDraw();
};