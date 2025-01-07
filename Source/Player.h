#pragma once

#include "System/ModelRenderer.h"
#include "character.h"
#include "ProjectileManager.h"

// �v���C���[
class Player : public Character
{
protected:
	void OnLanding()override;

public:
	Player() {};
	~Player() override {};

	void Initialize();
	void Finalize();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc,ModelRenderer* renderer);

	//�f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	void SetStartPos(DirectX::XMFLOAT3 State);

	bool pupCool() { return IsMove; };
	bool ISGoal() { return IsGoal; };
	bool IsDead() { return IsLive; };

	void DeadTime(float elapsedTime);

private:
	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	DirectX::XMFLOAT3 LastVec;

	//�e��Vs�G�l�~�[�����蔻��
	void CollisionProjectileVsEnemies();

	// �ړ����͏���
	void InputMove(float elapsedTime);

	// �W�����v���͏���
	void InputJump();


	// �e�ۓ��͏���
	void InputProjectile();

	void CollisionPlayerVSEnemys();

private:
	Model* model = nullptr;

	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGui�ŕψڂ��������A�����o�ϐ��ɕύX
	int JumpCount = 0;
	int JumpLimit = 2;

	bool IsControllable = true; // ������Ԃł͑���\

	float DeadTimer = 0;

	ProjectileManager projectileManager;
};	

