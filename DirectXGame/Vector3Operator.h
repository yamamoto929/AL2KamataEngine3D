#pragma once
#include "KamataEngine.h"
#include "math/Vector3.h"

KamataEngine::Vector3 operator+=( KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhs);
KamataEngine::Vector3 operator-=( KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhs);
KamataEngine::Vector3 operator*=( KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhs);
KamataEngine::Vector3 operator/=( KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhs);