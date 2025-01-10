#include "Player.h"
#include "System/Input.h"
#include <imgui.h>
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include <DirectXMath.h>
#include "ProjectileHoming.h"
#include "System/Graphics.h"
#include "Stage.h"

int Player::GoalNum = 0;

void Player::OnLanding()
{
	JumpCount = 0;
}

// �R���X�g���N�^
//Player::Player()
//{
//	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
//
//	// ���f�����傫���̂ŃX�P�[�����O
//	scale.x = scale.y = scale.z = 0.01f;
//}
//// �f�X�g���N�^
//Player::~Player()
//{
//	delete model;
//}


// �R���X�g���N�^�̑���
void Player::Initialize()
{
	model = new Model("Data/Model/Player/Player.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.2f;

	position.y = 5;

	Startpos = position;
}

// �f�X�g���N�^�̑���
void Player::Finalize()
{
	delete model;
}

void Player::Update(float elapsedTime)
{
	if (!IsGoal)
	{
		// �ړ����͏���
		InputMove(elapsedTime);
		//InputJump();
		UpdateVelocity(elapsedTime);
	}

	DeadTime(elapsedTime);
	projectileManager.Update(elapsedTime);
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();
	// ���f���s��X�V
	model->UpdateTransform();

	CollisionPlayerVSEnemys();

	CollisionProjectileVsEnemies();

	if (Velocity.x > 0)
	{
		if (Velocity.x >= Velocity.z)
		{
			Velocity.z = 0;
			Velocity.x = moveSpeed * accel;
		}
	}
	if (Velocity.x < 0)
	{
		if (Velocity.x < Velocity.z)
		{
			Velocity.z = 0;
			Velocity.x = -moveSpeed * accel;
		}
	}
	if(Velocity.z > 0)
	{
		if (Velocity.x <= Velocity.z)
		{
			Velocity.x = 0;
			Velocity.z = moveSpeed * accel;
		}
	}
	if(Velocity.z < 0)
	{
		if (Velocity.x > Velocity.z)
		{
			Velocity.x = 0;
			Velocity.z = -moveSpeed * accel;
		}
	}

}

void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	renderer->Render(rc, transform, model, ShaderId::Lambert);

	projectileManager.Render(rc, renderer);
}

// �f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::InputFloat3("Position", &position.x);

			// ��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);

			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);

			// �X�P�[��
			ImGui::InputFloat3("Scale", &scale.x);

			// ��]�l
			//ImGui::InputFloat("rot", &rot);

		}
	}

	ImGui::End();
}

void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

void Player::DeadTime(float elapsedTime)
{
	if (IsMove && Velocity.x == 0 && Velocity.z == 0)
	{
		DeadTimer += elapsedTime;
		if (DeadTimer > 3)
		{
			IsLive = true;
		}
	}
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{

	// ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���Ȃ�悤�ɂ���

	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;

	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;

	//   // ���͏����擾
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();
	//float ay = gamePad.GetAxisLY();

	//// �J�����������擾
	//Camera& camera = Camera::Instance();
	//DirectX::XMFLOAT3 cameraRight = camera.GetRight();
	//DirectX::XMFLOAT3 cameraFront = camera.GetFront();

	//// �J����������P�ʃx�N�g���ɐ��K���iXZ���ʁj
	//DirectX::XMFLOAT3 moveVec = {
	//	cameraFront.x * ay + cameraRight.x * ax,
	//	0.0f,  // Y���͖���
	//	cameraFront.z * ay + cameraRight.z * ax
	//};

	//// �ړ��x�N�g���𐳋K��
	//float length = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
	//if (length > 0.0f) {
	//	moveVec.x /= length;
	//	moveVec.z /= length;
	//}

	//return moveVec;
}


void Player::CollisionProjectileVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	int projectileCount = projectileManager.GetprojectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* Pro = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* Ene = enemyManager.GetEnemy(j);
			DirectX::XMFLOAT3 OutP;
			if (Collision::InterctSpherVSCylinder(
				Pro->GetPosition(),
				Pro->GetRadius(),
				Ene->GetPosition(),
				Ene->GetRadius(),
				Ene->GetHeight(),
				OutP))
			{
				if (Ene->ApplyDamage(1, 30.0f))
				{
					//������΂�
					{
						//// �e�ۂ̕�������G�𐁂���΂��������v�Z
						DirectX::XMFLOAT3 ProPos = Pro->GetPosition();
						DirectX::XMFLOAT3 EnePos = Ene->GetPosition();
						DirectX::XMFLOAT3 Impulse = {
							EnePos.x - ProPos.x,
							EnePos.y - ProPos.y,
							EnePos.z - ProPos.z
						};

						// ���K�����Ĉ��̐�����΂����x��K�p
						float length = sqrtf(Impulse.x * Impulse.x + Impulse.y * Impulse.y + Impulse.z * Impulse.z);
						if (length > 0.0f)
						{
							Impulse.x /= length;
							Impulse.y /= length;
							Impulse.z /= length;
						}

						// ���x��K�p (������΂��̋����͒����\)
						float pushStrength = 10.0f; // ������΂��͂̋���
						Impulse.x *= pushStrength;
						Impulse.y *= pushStrength;
						Impulse.z *= pushStrength;

						Ene->AddImpulse(Impulse); // ������΂��x�N�g����K�p

					}

					Pro->Destroy();

				}
			}

		}
	}
}

// �ړ����͏���
void Player::InputMove(float elapsedTime)
{
	//�ړ��t���O�̊Ǘ�
	if (Startpos.x != position.x || Startpos.z != position.z)
	{
		IsMove = true;
	}

	//�ړ��J�nOK�@Enter�@OR�@Second3��
	if (IsKeyPressed(VK_RETURN) || OnMovingFloorTime > 3)
	{
		if (moveState == 0)
		{
			angle.y = round(angle.y / 90.0) * 90; // 90�x�P�ʂŊۂ߂�
			if (angle.y > 360) {
				angle.y -= 360;
			}
			else if (angle.y < -0) {
				angle.y += 360;
			}
		}
		moveState = 1;
	
	}

	// �i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = { 0,0,0 };
	if (!IsMove)
	{
		moveVec = GetMoveVec();
		OnMovingFloorTime += elapsedTime;
	}
	// �ړ�����
	if(moveState == 1)
	{
		if (Velocity.x != 0 || Velocity.z != 0 && !IsMove) moveVec = Velocity;
		Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	}
	// ���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);


}

void Player::InputJump()
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		
		if (JumpCount < JumpLimit)
		{
			Jump(jumpSpeed);
		}
		JumpCount++;
	}

}

void Player::InputProjectile()
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	if(gamepad.GetButtonDown() & GamePad::BTN_X)
	{
		DirectX::XMFLOAT3 dir = {0,0,0};
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);

		//���ˈʒu
		DirectX::XMFLOAT3 pos = {position.x,position.y,position.z };
		pos.y +=  height * 0.5f;


		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		DirectX::XMFLOAT3 dir;

		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);

		//���ˈʒu
		DirectX::XMFLOAT3 pos = { position.x,position.y,position.z };
		pos.y += height * 0.5f;

		//�^�[�Q�b�g�i�f�t�H���g�̓v���C���[�̑O���j
		DirectX::XMFLOAT3 target =
		{position.x + dir.x * 1000,
		 position.y + dir.y * 1000,
		 position.z + dir.z * 1000 };

		//��ԋ߂��̓G���^�[�Q�b�g��
		float dist = FLT_MAX;
		EnemyManager& enemymanager = EnemyManager::Instance();
		int enemyCount = enemymanager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)
		{
			//�G�Ƃ̋�������

			 // �G�̈ʒu���擾
			Enemy* enemy = enemymanager.GetEnemy(i);
			DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

			// �����̓����v�Z
			DirectX::XMVECTOR PPos = DirectX::XMLoadFloat3(&pos);
			DirectX::XMVECTOR EPos = DirectX::XMLoadFloat3(&enemyPos);

			DirectX::XMVECTOR D = DirectX::XMVectorSubtract(PPos, EPos);
			float dx = enemyPos.x - pos.x;
			float dy = enemyPos.y - pos.y;
			float dz = enemyPos.z - pos.z;
			float distSq = dx * dx + dy * dy + dz * dz;

			// �ŒZ�����̓G���X�V
			if (distSq < dist)
			{
				dist = distSq;
				target = enemyPos;
			}
		}

		// �^�[�Q�b�g��������Ȃ������ꍇ�͔��˒��~
		//if (dist == FLT_MAX)
		//{
		//	return; // �^�[�Q�b�g���Ȃ��ꍇ�̏���
		//}

		//����
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos,target);
	}
}


void Player::CollisionPlayerVSEnemys()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	int enmyCount = enemyManager.Instance().GetEnemyCount();

	for (int i = 0; i < enmyCount; i++)
	{
		Enemy* enemy = enemyManager.Instance().GetEnemy(i);
		DirectX::XMFLOAT3 outposition;
		//if (Collision::IntersectVsShaere(
		//	position, radius,
		//	enemy->GetPosition(), enemy->GetRadius(),
		//	outposition))
		//{
		// 
		//	enemy->SetPosition(outposition);
		//}
		if (Collision::IntersectVsSCylinder(
			position, radius, height,
			enemy->GetPosition(), enemy->GetRadius(),
			enemy->GetHeight(),
			outposition))
		{
			DirectX::XMFLOAT3 EP = enemy->GetPosition();
			DirectX::XMVECTOR PlayP = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR EneP = DirectX::XMLoadFloat3(&EP);

			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PlayP,EneP);
			Vec = DirectX::XMVector3Normalize(Vec);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, Vec);
			if (normal.y > 0.6f)
			{
				Jump(normal.y * (jumpSpeed));
			}
			else
			{
				enemy->SetPosition(outposition);
			}
		}
	}
}


void Player::SetStartPos(DirectX::XMFLOAT3 State)
{
	Startpos = State;
	position = Startpos;
}


