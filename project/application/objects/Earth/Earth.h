#pragma once

#include "3d/entityController/EntityController.h"

#include "VFX/particle/emitterController/EmitterController.h"


class Earth : public EntityController {
public:
	Earth() = default;
	~Earth() = default;

	void Initialize(const std::string& name)override;

	// 更新
	void Update()override;

	// 衝突コールバック関数
	void OnCollision([[maybe_unused]] Collider* other)override;

	// 移動制限
	void MoveLimit();


	// 初期位置に戻る
	void ReturnPosition();

	// 生死処理
	void UpdateLifeState();

	float& GetHp();

	void SetPraticle();

	bool GetIsHit() const { return isObjectHit; };

	void SetIsHit(bool hit) { isObjectHit = hit; }

	int GetHitLevel() { return objectHitLevel; }

	float GetAroundFrame() const;

private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="emitPraticle"></param> パーティクル名
	/// <param name="emitName"></param> エミッター名
	/// <param name="count"></param> 出現数
	/// <param name="size"></param> 大きさ(float)
	/// <param name="lifeTime"></param> 生存時間(Vector2){x = min,y = max}
	/// <param name="color"></param> カラー(Vector)　
	/// <param name="emit"></param> エミッタークラス
	void CreateEmit(const std::string praticleName, const std::string emitName, int count, float size,Vector2 lifeTime ,Vector3 color, EmitterController* emit);

	void EmitMinMax(const Vector3& pos,const Vector3& veloctiy, EmitterController* emit);

	void EmitDust(const Vector3& pos, const Vector3& veloctiy);

	static Vector3 ElementWiseMax(const Vector3& a, const Vector3& b) {
		return Vector3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
	}

	static Vector3 ElementWiseMin(const Vector3& a, const Vector3& b) {
		return Vector3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
	}
private:
	std::unique_ptr<EmitterController> emitterDustRed_; // 赤
	std::unique_ptr<EmitterController> emitterDustYellow_; // 黄色
	std::unique_ptr<EmitterController> emitterDustGray_; // 灰色
	std::unique_ptr<EmitterController> emitterDustBlack_; // 黒っぽい色



private:

	// 一周するフレーム
	const float aroundFrame_ = 300.0f;

	Vector3 velocity_{};

	// 移動できる範囲
	float stageWidth_ = 31.0f;
	float stageHeight_ = 17.0f;

	// 初期位置に戻ろうとする行動に入る時間
	float kReturnMoveTime_ = 3.0f;
	float returnMoveTimer_ = 0.0f;
	float returnSpeed_ = 0.9f;

	float HP_ = 100.0f;

	bool isAlive_ = true;
	float inclination_ = 23.4f;
	float inclinationRadian_ = 0.0f;

	// 当たった物体の大きさ(これでカメラのシェイクの大きさが決まる)
	int objectHitLevel = 0;
	bool isObjectHit = false;
};