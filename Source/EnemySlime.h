#pragma once

#include "System/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"

// �X���C��
class EnemySlime : public Enemy
{
public:
	EnemySlime();
	~EnemySlime() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer) override;

	void OnDead()override;
	//�f�o�b�N�v���~�e�B�u
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)override;
	//�ړ��͈͐ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
private:
	//�^�[�Q�b�g�̈ʒu�������_���Ɋ���U��
	void SetRandomTargetposition();
	//�ړI�n�ֈړ�
	void MoveToTarget(float elaspedTime, float moveSpeedRate, float turnSpeedRate);
	//�p�j�X�e�[�g�֑J��
	void SetWanderState();
	//�p�j�X�e�[�g�̍X�V
	void UpdateWanderState(float elapsedTime);
	//�ҋ@�X�e�[�g�Ɉڍs
	void SetIdleState();
	//�ҋ@�X�e�[�g�X�V
	void UpdateIdleState(float elapsedTime);
	//�v���C���[���G
	bool SearthPlayer();
	//Attack�ڍs
	void SetAttackState();
	//Attack�X�e�[�g�X�V
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
