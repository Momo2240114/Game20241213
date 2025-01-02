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

	static Player& Instance()
	{
		static Player instance;
		return instance;
	}

	void Initialize();
	void Finalize();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc,ModelRenderer* renderer);

	//�f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

	void SetStaetPos(DirectX::XMFLOAT3 State);
private:
	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;

	//�e��Vs�G�l�~�[�����蔻��
	void CollisionProjectileVsEnemies();

	// �ړ����͏���
	void InputMove(float elapsedTime);

	// �W�����v���͏���
	void InputJump();

	// �e�ۓ��͏���
	void InputProjectile();

	void CollisionPlayerVSEnemys();

	void pupCool(float elapsedTime);
private:
	Model* model = nullptr;
	float		moveSpeed = 4.0f;
	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGui�ŕψڂ��������A�����o�ϐ��ɕύX
	int JumpCount = 0;
	int JumpLimit = 2;

	bool IsLive = false;
	bool IsGoal = false;
	bool IsMove = false;

	float moveStateTimer = 0;
	const int PopTime = 20;

	ProjectileManager projectileManager;
};	

