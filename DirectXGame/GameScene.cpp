#include "GameScene.h"
#include "WorldMatrixUpdate.h"
#include "imgui.h"
#include <vector>
using namespace KamataEngine;

GameScene::~GameScene() {
	delete modelPlayer_;
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
	delete mapChipField_;
	delete cameraController_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete deathParticles_;
}
// 初期化
void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	textureHandleBlock_ = TextureManager::Load("uvChecker.png");

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelBlock_ = Model::CreateFromOBJ("box", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();

	camera_ = new Camera();
	camera_->Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();
	skydome_->Initialize(camera_, modelSkydome_);

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 14);

	player_ = new Player();
	player_->Initialize(modelPlayer_, camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	deathParticles_ = new DeathParticles();
	deathParticles_->Initialize(modelDeathParticles_, camera_, playerPosition);

	// for (Enemy* enemy : enemies_) {
	// enemy = new Enemy();
	// enemy->Initialize(modelEnemy_, camera_, Vector3{10.0f, 1.0f, 0.0f});
	//}

	for (size_t i = 0; i < kEnemyMax; ++i) {
		Enemy* newEnemy = new Enemy();
		uint32_t enemyPositionXIndex = static_cast<uint32_t>(i) + 20;
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(enemyPositionXIndex, 18);
		newEnemy->Initialize(modelEnemy_, camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	cameraController_ = new CameraController();
	cameraController_->Initialize(camera_);
	cameraController_->SetTarget(player_);
	CameraController::Rect movableArea = {11.0f, 88.0f, 6.0f, 20.0f};
	cameraController_->SetMovableArea(movableArea);
	cameraController_->Reset();

	phase_ = Phase::kPlay;
};

// 更新処理
void GameScene::Update() {
	switch (phase_) {
	case Phase::kPlay:
		// #ifdef _DEBUG
		//		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//			isDebugCameraActive_ = !isDebugCameraActive_;
		//		}
		// #endif
		
		//  天球
		skydome_->Update();

		// プレイヤー
		player_->Update();

		// 敵(複数)
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// カメラコントローラー
		cameraController_->Update();

		// カメラ
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_->matView = debugCamera_->GetCamera().matView;
			camera_->matProjection = debugCamera_->GetCamera().matProjection;
			camera_->TransferMatrix();
		} else {
			camera_->UpdateMatrix();
		}

		// ブロック
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				WorldMatrixUpdate(*worldTransformBlock);
			}
		}

		// すべての当たり判定
		CheckAllCollisions();
		break;

	case Phase::kDead:
		// 天球
		skydome_->Update();

		// 敵(複数)
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 死亡時のパーティクル
		if (deathParticles_ != nullptr) {
			deathParticles_->Update();
		}

		// カメラ
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_->matView = debugCamera_->GetCamera().matView;
			camera_->matProjection = debugCamera_->GetCamera().matProjection;
			camera_->TransferMatrix();
		} else {
			camera_->UpdateMatrix();
		}

		// ブロック
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				WorldMatrixUpdate(*worldTransformBlock);
			}
		}

		DebugText::GetInstance()->ConsolePrintf("kDead\n");
		break;
	}

	ChangePhase();
};

// 描画処理
void GameScene::Draw() {

	Model::PreDraw();
	// model_->Draw(worldTransform_, camera_, textureHandle_);
	player_->Draw();
	skydome_->Draw();

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, *camera_);
		}
	}

	if (deathParticles_ != nullptr) {
		deathParticles_->Draw();
	}

	Model::PostDraw();
};

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetBlockHorizontal();
	// 要素数を変更する
	// 列数を設定 (縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定 (横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region checkPlayer-EnemyCollision
	{
		AABB aabb1, aabb2;

		aabb1 = player_->GetAABB();

		for (Enemy* enemy : enemies_) {

			aabb2 = enemy->GetAABB();

			if (CheckAABBCollision(aabb1, aabb2)) {
				player_->OnCollision(enemy);
				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			phase_ = Phase::kDead;
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_->Initialize(modelDeathParticles_, camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDead:
		break;
	}
}