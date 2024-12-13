#pragma once

#include "System/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"

// スライム
class EnemySlime : public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	void OnDead()override;
	//デバックプリミティブ
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;
	//移動範囲設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
private:
	//ターゲットの位置をランダムに割り振り
	void SetRandomTargetposition();
	//目的地へ移動
	void MoveToTarget(float elaspedTime, float moveSpeedRate, float turnSpeedRate);
	//徘徊ステートへ遷移
	void SetWanderState();
	//徘徊ステートの更新
	void UpdateWanderState(float elapsedTime);
	//待機ステートに移行
	void SetIdleState();
	//待機ステート更新
	void UpdateIdleState(float elapsedTime);
	//プレイヤー索敵
	bool SearthPlayer();
	//Attack移行
	void SetAttackState();
	//Attackステート更新
	void UpdateAttackState(float elapsedTime);
private:
	enum class State
	{
		Wander,
		Idle,
		Attack
	};	


	Model* model = nullptr;
	State   state = State::Wander;
	DirectX::XMFLOAT3 targetpos = { 0,0,0 };
	DirectX::XMFLOAT3 TerritoryOrigin = { 0,0,0 };
	float territoryRange = 10.0f;
	float moveSpeed = 2.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);

	float statetimer = 0.0f;
	float SearthRange = 5.0f;
	ProjectileManager  projectileMana;
};
