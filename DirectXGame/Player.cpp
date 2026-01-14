#define NOMINMAX
#include "Player.h"
#include "Vector3Operator.h"
#include "WorldMatrixUpdate.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.translation_.y = 1.0f;
	//worldTransform_.translation_.x += 2.0f;
	camera_ = camera;
}

void Player::Update() {
	Move();

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.velocity = velocity_;

	CheckMapCollidion(collisionMapInfo);

	//worldTransform_.translation_ += velocity_;

	MoveByResult(collisionMapInfo);

	OnContactCeiling(collisionMapInfo);

	bool landing = false;
	if (velocity_.y < 0.0f) {

		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;

		float t = 1.0f - (turnTimer_ / kTimeTurn);

		float easedT = 1.0f - powf(1.0f - t, 5.0f);
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた目標角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * easedT;
	}

	WorldMatrixUpdate(worldTransform_);
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle_); }

void Player::Move() {
	if (onGround_) {
		// 移動入力
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速/減速
			velocity_ += acceleration;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, kLimitFallSpeed);
	}
}

void Player::CheckMapCollidion(CollisionMapInfo& info) {
	CheckMapCollidionTop(info);
	CheckMapCollidionBottom(info);
	CheckMapCollidionLeft(info);
	CheckMapCollidionRight(info);
}

// 上
void Player::CheckMapCollidionTop(CollisionMapInfo& info) { 
	if (info.velocity.y <= 0.0f) {
		return;
	}

	std::array<Vector3, kNUMCORNER> positionsNew{}; 

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_+info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit =false;
	// 左上点の判定
	MapChipField::IndexSet indexSetLeftTop;
	indexSetLeftTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLEFTTOP]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSetLeftTop.xIndex, indexSetLeftTop.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit =true;
	}

	// 右上点の判定
	MapChipField::IndexSet indexSetRightTop;
	indexSetRightTop = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRIGHTTOP]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSetRightTop.xIndex, indexSetRightTop.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		// めり込みを排除するために移動量を設定する
		MapChipField::IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLEFTTOP]);
		
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kHeight / 2.0f + kBlank));

		info.isCollideCeiling = true;
	}
};

// 下
void Player::CheckMapCollidionBottom(CollisionMapInfo& info) {
	info;
	info;
};

// 左
void Player::CheckMapCollidionLeft(CollisionMapInfo& info) {
	info;
	info;
};

// 右
void Player::CheckMapCollidionRight(CollisionMapInfo& info) {
	info;
	info;
};

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNUMCORNER] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f,  0.0f}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0.0f}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}  // kLeftTop
	};
	return center+offsetTable[static_cast<uint32_t>(corner)];
};

void Player::MoveByResult(const CollisionMapInfo& info) { 
	worldTransform_.translation_ += info.velocity; 
};

void Player::OnContactCeiling(const CollisionMapInfo& info) {
	if (info.isCollideCeiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}
};