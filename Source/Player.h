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

	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	void Initialize();
	void Finalize();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc,ModelRenderer* renderer);

	//デバッグ用GUI描画
	void DrawDebugGUI();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	void SetStaetPos(DirectX::XMFLOAT3 State);
private:
	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;

	//弾丸Vsエネミー当たり判定
	void CollisionProjectileVsEnemies();

	// 移動入力処理
	void InputMove(float elapsedTime);

	// ジャンプ入力処理
	void InputJump();

	// 弾丸入力処理
	void InputProjectile();

	void CollisionPlayerVSEnemys();

	void pupCool(float elapsedTime);
private:
	Model* model = nullptr;
	float		moveSpeed = 4.0f;
	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGuiで変移を見たく、メンバ変数に変更
	int JumpCount = 0;
	int JumpLimit = 2;

	bool IsLive = false;
	bool IsGoal = false;
	bool IsMove = false;

	float moveStateTimer = 0;
	const int PopTime = 20;

	ProjectileManager projectileManager;
};	

