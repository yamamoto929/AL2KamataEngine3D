#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>

using namespace KamataEngine;
namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {
    mapChipData_.data.clear();
	mapChipData_.data.resize(kBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string filepath) {
	ResetMapChipData();

	std::ifstream file;
	file.open(filepath);
	assert(file.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream lineStream(line);
		for (uint32_t j = 0; j < kBlockHorizontal; ++j) {
			std::string word;
			std::getline(lineStream, word, ',');
			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kBlockHorizontal-1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kBlockVirtical-1 < yIndex) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Vector3(
		static_cast<float>(kBlockWidth * xIndex),
		static_cast<float>(kBlockHeight * (kBlockVirtical - 1 - yIndex)),
		0.0f
	);
}