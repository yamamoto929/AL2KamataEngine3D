#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() { delete model_; }

// 初期化
void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();

	worldTransform_ = new KamataEngine::WorldTransform();

	worldTransform_->Initialize();

	camera_ = new KamataEngine::Camera();

	camera_->Initialize();
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, camera_);
};

// 更新処理
void GameScene::Update() { player_->Update(); };

// 描画処理
void GameScene::Draw() {
	Model::PreDraw();
	// model_->Draw(worldTransform_, camera_, textureHandle_);
	player_->Draw();
	Model::PostDraw();
};