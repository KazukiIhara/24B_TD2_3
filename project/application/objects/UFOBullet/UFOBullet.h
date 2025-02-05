#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"

class DamagePieceManager;
class UFOBullet :public EntityController {
public:
	UFOBullet();
	~UFOBullet();

	void Initialize(const std::string& name)override;
	// 更新
	void Update()override;
	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;
	void SetVelocity(const Vector3& velocity);
	bool GetIsAlive()const;
	void MoveLimit();
	void SetDamagePieceManager(DamagePieceManager* damagePieceManager);


	void SetPraticle(int count);

	void KillMe();
private:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="emitPraticle"></param> パーティクル名
	/// <param name="emitName"></param> エミッター名
	/// <param name="count"></param> 出現数
	/// <param name="size"></param> 大きさ(float)
	/// <param name="color"></param> カラー(Vector)　
	/// <param name="emit"></param> エミッタークラス
	void CreateEmit(const std::string praticleName, const std::string emitName, int count, float size, Vector2 lifeTime, Vector3 color, EmitterController* emit);

	//void EmitFragment(const Vector3& velo);
	void EmitDust(const Vector3& pos, const Vector3& veloctiy);

	void EmitMinMax(const Vector3& pos, const Vector3& veloctiy, EmitterController* emit);

	void EmitDie();

	static Vector3 ElementWiseMax(const Vector3& a, const Vector3& b) {
		return Vector3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
	}

	static Vector3 ElementWiseMin(const Vector3& a, const Vector3& b) {
		return Vector3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
	}
	void ShotEmit();
private:
	Vector3 velocity_ = {};
	bool isAlive_ = true;

	float stageWidth_ = 48.0f;
	float stageHeight_ = 27.0;

	DamagePieceManager* damagePieceManager_ = nullptr;

	// 番号
	uint32_t particleNumber_ = 0;
private:
	// エミッター
	std::unique_ptr<EmitterController> emitterExplosionFire_;
	std::unique_ptr<EmitterController> emitterExplosionFireYellow_;
	std::unique_ptr<EmitterController> emitterExplosionDust_;

	std::unique_ptr<EmitterController> emitterShotDust_;

};
