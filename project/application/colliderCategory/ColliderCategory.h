#pragma once

// コライダーのカテゴリ
enum class ColliderCategory {
	None,
	Player,
	Earth,
	Fragment,
	Meteorite,
	Bump,
	DamagePiece, // 破損片
};