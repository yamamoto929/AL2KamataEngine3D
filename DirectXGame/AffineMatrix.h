#pragma once
#include "KamataEngine.h"
#include <math/Vector3.h>
#include <math/Matrix4x4.h>


KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);


