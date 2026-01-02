#include "GameScene.h"
#include <vector>

using namespace KamataEngine;

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

GameScene::~GameScene() {
	delete model_;
	delete modelBlock_;

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
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
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new KamataEngine::WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
};

// 更新処理
void GameScene::Update() {
	player_->Update();
	// ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		/*アフィン変換行列の作成*/
		Matrix4x4 affineMatrix = MakeAffineMatrix(worldTransformBlock->scale_,worldTransformBlock->rotation_,worldTransformBlock->translation_);

		worldTransformBlock->matWorld_ = affineMatrix;
		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}
};

// 描画処理
void GameScene::Draw() {
	Model::PreDraw();
	// model_->Draw(worldTransform_, camera_, textureHandle_);
	// player_->Draw();
	// ブロックの描画
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		modelBlock_->Draw(*worldTransformBlock, *camera_);
	}
	Model::PostDraw();
};

// アフィン変換行列を生成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = {};
	// 回転行列の計算 
	float sx = sinf(rotate.x), cx = cosf(rotate.x);
	float sy = sinf(rotate.y), cy = cosf(rotate.y);
	float sz = sinf(rotate.z), cz = cosf(rotate.z);

	// 回転行列
	Matrix4x4 matRot = {};
	matRot.m[0][0] = cy * cz;                matRot.m[0][1] = cy * sz;                matRot.m[0][2] = -sy;

	matRot.m[1][0] = sx * sy * cz - cx * sz; matRot.m[1][1] = sx * sy * sz + cx * cz; matRot.m[1][2] = sx * cy;

	matRot.m[2][0] = cx * sy * cz + sx * sz; matRot.m[2][1] = cx * sy * sz - sx * cz; matRot.m[2][2] = cx * cy;

	                                                                                                            matRot.m[3][3] = 1.0f;

	// スケールを回転行列に適用
	for (int i = 0; i < 3; ++i) {
		result.m[0][i] = matRot.m[0][i] * scale.x;
		result.m[1][i] = matRot.m[1][i] * scale.y;
		result.m[2][i] = matRot.m[2][i] * scale.z;
	}

	// 平行移動を代入
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}