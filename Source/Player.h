#pragma once

#include "System/ModelRenderer.h"
#include "character.h"
#include "ProjectileManager.h"

// プレイヤー
class Player : public Character
{
protected:
	void OnLanding()override;


private:

	Player() {};
	~Player() override {};
public:
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

private:
	Model* model = nullptr;
	float		moveSpeed = 1.5f;
	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGuiで変移を見たく、メンバ変数に変更
	int JumpCount = 0;
	int JumpLimit = 2;
	
	ProjectileManager projectileManager;
};	

