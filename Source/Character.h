#pragma once

#include <DirectXMath.h>
#include "System/ShapeRenderer.h"

//�L�����N�^�[
class Character
{
public:
	Character() {};
	virtual ~Character() {};

	//�s��X�V����
	void UpdateTransform();

	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	float GetHeight()const { return height; };
	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �X�P�[���擾
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	void SetBlockAngle(const DirectX::XMFLOAT3& angle) { this->HitBlockAngle = angle; }

	float GetRadius() const { return radius; }

	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	bool IsGround()const { return isGround; };

	bool ApplyDamage(int damage ,float invincibletime);

	void HandleBlockCollision(int blockID, const DirectX::XMFLOAT3& hitPosition, const DirectX::XMFLOAT3& hitNormal);
private:
	//�����̑��͍X�V
	void UpdateVerticalVelocity(float elapsedTime);	
	//�����̑��͍X�V
	void UpdateHorizonVelocity(float elapsedTime);
	//�����̈ړ��X�V
	void UpdateVerticalMove(float elapsedTime);
		//�����̈ړ��X�V
	void UpdateHorizonMove(float elapsedTime);


protected:
	DirectX::XMFLOAT3	position = {0,0,0};
	DirectX::XMFLOAT3	Previousposition = {0,0,0};
	DirectX::XMFLOAT3	angle = {0,0,0};
	DirectX::XMFLOAT3	scale = {1,1,1};
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	virtual void OnDamaged() {};
	virtual void OnDead() {};

	float gravity = -30.0f;

	DirectX::XMFLOAT3 Velocity = { 0,0,0 };

	DirectX::XMFLOAT3 HitBlockAngle = { 0,0,0 };

	bool isGround = false;

	virtual void OnLanding() {};

	void Jump(float speed);



	void UpdateVelocity(float elapsedTime);

	void Updateinvincibletimer(float elapsedTime);

	float height = 2.0f;
	float radius = 0.5f;

	int Health = 5;

	int invincibletimer = 1.0f;

	float friction = 12.0f;

	// ���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);
	// �ړ�����
	void Move(float elapsedTime, float vx, float vz, float speed);

	float acceleration = 50.0f;
	float MaxMoveSpeed = 0.1f;
	float MoveVecX = 0.0f;
	float MoveVecZ = 0.0f;

	float airControl = 1.3f;

	float stepOffset = 1.0f;
};

