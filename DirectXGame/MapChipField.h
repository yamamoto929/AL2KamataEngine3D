#pragma once
#include "KamataEngine.h"
#include "math/Vector3.h"
enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

///< summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
private:
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kBlockVirtical = 20; // 縦
	static inline const uint32_t kBlockHorizontal = 100; // 横

	MapChipData mapChipData_;

public:
	uint32_t GetBlockVirtical() { return kBlockVirtical; }
	uint32_t GetBlockHorizontal() { return kBlockHorizontal; }
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string filepath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
};
