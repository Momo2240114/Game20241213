#pragma once

#include <DirectXMath.h>
#include "System/ShapeRenderer.h"

//キャラクター
class Character
{
public:
	Character() {};
	virtual ~Character() {};

	//行列更新処理
	void UpdateTransform();

	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	float GetHeight()const { return height; };
	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 回転取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 回転設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// スケール取得
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	void SetBlockAngle(const DirectX::XMFLOAT3& angle) { this->HitBlockAngle = angle; }

	float GetRadius() const { return radius; }

	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	bool IsGround()const { return isGround; };

	bool ApplyDamage(int damage, float invincibletime);

	void HandleBlockCollision(int blockID, const DirectX::XMFLOAT3& hitPosition, const DirectX::XMFLOAT3& hitNormal);


	static int ISGoalNum() { return GoalNum; }

private:
	static int GoalNum;

	//垂直の速力更新
	void UpdateVerticalVelocity(float elapsedTime);
	//水平の速力更新
	void UpdateHorizonVelocity(float elapsedTime);
	//垂直の移動更新
	void UpdateVerticalMove(float elapsedTime);
	//垂直の移動更新
	void UpdateHorizonMove(float elapsedTime);


	void HitAngleCheck(float HitAngle);
protected:
	DirectX::XMFLOAT3	TargetPosition = { 0,0,0 };
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	Startpos = { 0,0,0 };
	DirectX::XMFLOAT3	Previousposition = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	void revisionVelocity() {};
	virtual void OnDamaged() {};
	virtual void OnDead() {};



	float gravity = -30.0f;

	bool IsLive = false;
	bool IsGoal = false;
	bool IsMove = false;

	DirectX::XMFLOAT3 Velocity = { 0,0,0 };

	DirectX::XMFLOAT3 OldVelocity = { 0,0,0 };

	DirectX::XMFLOAT3 HitBlockAngle = { 0,0,0 };

	DirectX::XMFLOAT3 LastHitPos = {0,0,0}; // 前回踏んだブロックの情報

	bool isGround = false;

	virtual void OnLanding() {};

	void Jump(float speed);

	int StopState = 0;

	void UpdateVelocity(float elapsedTime);

	void Updateinvincibletimer(float elapsedTime);

	float height = 2.0f;
	float radius = 0.25f;
	float WarpCool = 0;

	int Health = 5;

	int invincibletimer = 1.0f;

	float friction = 12.0f;

	float OnMovingFloorTime = 0;
	// 旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	// 移動処理
	void Move(float elapsedTime, float vx, float vz, float speed);

	float acceleration = 50.0f;
	float accel = 1.0f;
	float MaxMoveSpeed = 0.1f;
	float MoveVecX = 0.0f;
	float MoveVecZ = 0.0f;
	float moveSpeed = 3.0f;
	float airControl = 1.3f;

	int moveState = 0;
	int EvolutionType = 0;

	float stepOffset = 1.0f;
};

