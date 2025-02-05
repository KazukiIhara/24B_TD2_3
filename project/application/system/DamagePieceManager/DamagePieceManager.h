#pragma once
#include "objects/Fragment/Fragment.h"

#include <vector>
#include <list>

#include "application/objects/DamagePiece/DamagePiece.h"



class DamagePieceManager
{
public:
	void Initialize();

	void Update();

	void AddDamagePiece(const Vector3& popTranslate, const Vector3& velo = {},
		const float& renge = {}, const bool& isReduction = false,
		const Vector2& scaleMinMax = {},const Vector4& color = {}, 
		const Vector2& lifeMInMaxTime = {}, const bool isRand = true, const int numPiece = {});

	

private:
	// かけらのリスト
	std::list<std::unique_ptr<DamagePiece>> damagePiece_;

};

