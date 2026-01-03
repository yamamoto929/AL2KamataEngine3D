#include "WorldMatrixUpdate.h"
#include "AffineMatrix.h"
using namespace KamataEngine;

void WorldMatrixUpdate(WorldTransform& worldTransform) {
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	worldTransform.TransferMatrix();
}