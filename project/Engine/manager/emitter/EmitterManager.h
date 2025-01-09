#pragma once

// C++
#include <map>
#include <string>
#include <memory>

#include "VFX/particle/emitter/Emitter.h"

class ParticleManager;

class EmitterManager {
public:
	EmitterManager() = default;
	~EmitterManager() = default;

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 終了
	void Finalize();

	// コンテナをクリア
	void ClearContainer();

	// エミッターを新規作成
	std::string CreateEmitter(const std::string& name, const EulerTransform3D& transform);

	// 作成済みのエミッターを検索
	Emitter* Find(const std::string& name);

private:
	// エミッターデータコンテナ
	std::map <std::string, std::unique_ptr<Emitter>> emitters_;

};