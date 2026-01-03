#include "AffineMatrix.h"
using namespace KamataEngine;

// アフィン変換行列を生成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = {};
	// 回転行列の計算
	float sx = sinf(rotate.x), cx = cosf(rotate.x);
	float sy = sinf(rotate.y), cy = cosf(rotate.y);
	float sz = sinf(rotate.z), cz = cosf(rotate.z);

	// 回転行列
	Matrix4x4 matRot = {};
	matRot.m[0][0] = cy * cz;
	matRot.m[0][1] = cy * sz;
	matRot.m[0][2] = -sy;

	matRot.m[1][0] = sx * sy * cz - cx * sz;
	matRot.m[1][1] = sx * sy * sz + cx * cz;
	matRot.m[1][2] = sx * cy;

	matRot.m[2][0] = cx * sy * cz + sx * sz;
	matRot.m[2][1] = cx * sy * sz - sx * cz;
	matRot.m[2][2] = cx * cy;

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