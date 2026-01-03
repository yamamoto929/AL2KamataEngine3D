#include "GameScene.h"
#include <vector>
#include "WorldMatrixUpdate.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete model_;
	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete camera_;
	delete worldTransform_;
	delete player_;
	delete skydome_;
	delete modelSkydome_;
}

// 初期化
void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	textureHandleBlock_ = TextureManager::Load("uvChecker.png");

	model_ = Model::Create();
	modelBlock_ = Model::Create();

	worldTransform_ = new KamataEngine::WorldTransform();
	worldTransform_->Initialize();

	camera_ = new KamataEngine::Camera();
	camera_->Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, camera_);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する
	// 列数を設定 (縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定 (横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (j % 2 == 0 && i % 2 == 1) {
				continue;
			} else if (j % 2 == 1 && i % 2 == 0) {
				continue;
			}
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}

	debugCamera_ = new DebugCamera(1280, 720);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();
	skydome_->Initialize(camera_, modelSkydome_);
};

// 更新処理
void GameScene::Update() {
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif
	player_->Update();
	skydome_->Update();
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			WorldMatrixUpdate(*worldTransformBlock);
		}
	}

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_->matView = debugCamera_->GetCamera().matView;
		camera_->matProjection = debugCamera_->GetCamera().matProjection;
		camera_->TransferMatrix();
	} else {
		camera_->UpdateMatrix();
	}
};

// 描画処理
void GameScene::Draw() {
	
	Model::PreDraw();
	// model_->Draw(worldTransform_, camera_, textureHandle_);
	// player_->Draw();
	skydome_->Draw();
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, *camera_);
		}
	}
	
	Model::PostDraw();
};

