#include "EnemySlime.h"
#include "MathUtiles.h"
#include "Player.h"
#include "ProjectileStraight.h"

// �R���X�g���N�^
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.01f;
	height = 1.0f;
	radius = 0.5f;

	SetWanderState();
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
	delete model;
}

// �X�V����
void EnemySlime::Update(float elapsedTime)
{
	switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;	
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;	
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform();

	UpdateVelocity(elapsedTime);

	Updateinvincibletimer(elapsedTime);

	projectileMana.Update(elapsedTime);
}

// �`�揈��
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
	projectileMana.Render(rc, renderer);
}

void EnemySlime::OnDead()
{
	Destroy();
}

//�f�o�b�N�v���~�e�B�u
void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::RenderDebugPrimitive(rc, renderer);

	//�ړ��͈͂��f�o�b�O�~���`��
	renderer->RenderCylinder(rc, TerritoryOrigin, territoryRange, 1.0f,
		DirectX::XMFLOAT4(0, 1, 0, 1));

	renderer->RenderSphere(rc,targetpos,1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	//���G�͈͂��f�o�b�O�~���`��
	renderer->RenderCylinder(rc, position, SearthRange, 1.0f,
		DirectX::XMFLOAT4(1, 0, 0, 1));

	
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	TerritoryOrigin = origin;
	territoryRange = range;
}

void EnemySlime::SetRandomTargetposition()
{
	float theta = MathUtiles::RandomRange(-DirectX::XM_PI,DirectX::XM_PI);
	float range = MathUtiles::RandomRange(0.0f,territoryRange);

	targetpos.x = TerritoryOrigin.x + sinf(theta) * range;
	targetpos.y = TerritoryOrigin.y;
	targetpos.z = TerritoryOrigin.z + cosf(theta) * range;


}

void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//�^�[�Q�b�g�����̐M�x�N�g�����Z�o
	float vx = targetpos.x - position.x;
	float vz = targetpos.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//�ړ�����
	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}

void EnemySlime::SetWanderState()
{
	state = State::Wander;

	//�ړI�n�ݒ�
	SetRandomTargetposition();

}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//�v���C���[�����G
	if (SearthPlayer())
	{
		//�͈͓��Ƀv���C���[�������Attack�X�e�[�g��
		SetAttackState();
	}

	//�ړI�n�܂ł�XZ���ʂł̔���
	float vx = targetpos.x - position.x;
	float vz = targetpos.z - position.z;
	float distsq =vx * vx + vz * vz;
	if (distsq <= radius * radius)
	{
		//���̖ڕW
		/*SetRandomTargetposition();*/
		//�ҋ@�ɑJ��
		SetIdleState();
	}
	MoveToTarget(elapsedTime, 1.0f, 1.0f);


}

void EnemySlime::SetIdleState()
{
	state = State::Idle;

	statetimer = MathUtiles::RandomRange(3.0, 5.0f);

}

void EnemySlime::UpdateIdleState(float elapsedTime)
{
	//�v���C���[�����G
	if (SearthPlayer())
	{
		//�͈͓��Ƀv���C���[�������Attack�X�e�[�g��
		SetAttackState();
	}

	statetimer -= elapsedTime;
	if (statetimer < 0.0f)
	{
		SetWanderState();
	}
}

bool EnemySlime::SearthPlayer()
{
	//�v���C���[�Ƃ̍��፷���l�����ĂRD�ł̋�������
	const DirectX::XMFLOAT3& playerPos = { 0,0,0 }; //Player::Instance().GetPosition();
	float vx = playerPos.x - position.x;
	float vy = playerPos.y - position.y;
	float vz = playerPos.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < SearthRange )
	{
		float  distXZ = sqrtf(vx * vx + vz + vz);
		//�P�ʉ�
		vx /= distXZ;
		vz /= distXZ;
		//�O���x�N�g��
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		//���ύ��őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);

		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

void EnemySlime::SetAttackState()
{
	state = State::Attack;

	statetimer = 0.0f;
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
	////�ړI�n���v���C���[��
	//targetpos = Player::Instance().GetPosition();
	////�ړI�n�ֈړ�
	//MoveToTarget(elapsedTime, 0.2f, 1.0f);

	////�^�C�}�[����
	//statetimer -= elapsedTime;
	//if (statetimer < 0.0f)
	//{
	//	DirectX::XMFLOAT3 dir;
	//	dir.x = sinf(angle.y);
	//	dir.y = 0;
	//	dir.z = cosf(angle.y);

	//	//���ˈʒu
	//	DirectX::XMFLOAT3 pos = { position.x,position.y,position.z };
	//	pos.y += height * 0.5f;


	//	ProjectileStraight* projectile = new ProjectileStraight(&projectileMana);
	//	projectile->Launch(dir, pos);

	//	statetimer = 2.0f;
	//}

	//if (!SearthPlayer())
	//{
	//	SetIdleState();
	//}
}


