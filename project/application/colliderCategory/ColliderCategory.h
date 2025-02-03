#pragma once

// コライダーのカテゴリ
enum class ColliderCategory {
	None,
	Player,
	Moon,
	Fragment,
	Meteorite,
	Bump,
	DamagePiece, // 破損片
	UFO,
	UFOBullet,
};