#pragma once

#include "System/ModelRenderer.h"
#include "character.h"
#include "ProjectileManager.h"

// �v���C���[
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

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc,ModelRenderer* renderer);

	//�f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;

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

private:
	Model* model = nullptr;
	float		moveSpeed = 1.5f;
	float		turnSpeed = DirectX::XMConvertToRadians(720);
	float jumpSpeed = 8.0f;
	//float rot = 0.0f;//ImGui�ŕψڂ��������A�����o�ϐ��ɕύX
	int JumpCount = 0;
	int JumpLimit = 2;
	
	ProjectileManager projectileManager;
};	

