#pragma once

#include "System/ModelRenderer.h"
#include "character.h"
#include "ProjectileManager.h"

// プレイヤー
class Player : public Character
{
protected:
	void OnLanding()override;

public:
	Player() {};
	~Player() override {};

	void Initialize();
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc,ModelRenderer* renderer);

	//デバッグ用GUI描画
	void DrawDebugGUI();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	void SetStartPos(DirectX::XMFLOAT3 State);

	bool pupCool() { return IsMove; };
	bool ISGoal() { return IsGoal; };
	bool IsDead() { return IsLive; };

	void DeadTime(float elapsedTime);

private:
	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	DirectX::XMFLOAT3 LastVec;

	//弾丸Vsエネミー当たり判定
	void CollisionProjectileVsEnemies();

	// 移動入力処理
	void InputMove(float elapsedTime);

	// ジャンプ入力処理
	void InputJump();


	// 弾丸入力処理
	void InputProjectile();

	void CollisionPlayerVSEnemys();

private:
	Model* model = nullptr;

	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGuiで変移を見たく、メンバ変数に変更
	int JumpCount = 0;
	int JumpLimit = 2;

	bool IsControllable = true; // 初期状態では操作可能

	float DeadTimer = 0;

	ProjectileManager projectileManager;
};	

