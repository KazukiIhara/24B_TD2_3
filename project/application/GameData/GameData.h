#pragma once

#include <cstdint>

struct GameData {

	uint32_t days_;			// 日数
	uint32_t years_;		// 年数
	uint32_t score_;		// スコア
	uint32_t totalScore_;	// 合計スコア
	uint32_t fragmentNum_;	// 破壊された欠片数
	uint32_t meteoriteNum_;	// 破壊された隕石数
	uint32_t ufoNum_;		// 破壊されたUFO数
	bool bossDeath_;        // ボスが死んだか

};