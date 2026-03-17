#pragma once
//#include "math/Matrix4x4.h"
//#include "math/Vector3.h"

#include "KamataEngine.h"
#include "Vector3Operator.h"
#include <cmath>
#include <cassert>

KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4 matrix);
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian);