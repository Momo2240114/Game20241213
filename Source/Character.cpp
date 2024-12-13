#include "Character.h"
#include "Stage.h"

//�s��X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y,scale.z);

    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // �R�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//���͂ɗ͂�^����
	Velocity.x += impulse.x;
	Velocity.y += impulse.y;
	Velocity.z += impulse.z;
}

void Character::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Character::ApplyDamage(int damage, float invincibletime)
{
	if (damage == 0)return false;

	if (Health == 0)return false;

	if (invincibletimer > 0) return false;

	invincibletimer = invincibletime;

	Health -= damage;

	if (Health == 0)//���b����
	{
		OnDead();
	}
	else//�_���[�W����
	{
		OnDamaged();
	}
	//�̗͂��ς���
	return true;
}

void Character::HandleBlockCollision(int blockID, const DirectX::XMFLOAT3& hitPosition, const DirectX::XMFLOAT3& hitNormal)
{
	//switch (blockID) {
	//case 1:
	//	// �M�~�b�N1: ��) �L�����N�^�[�̓������~�߂�
	//	Velocity = { 0.0f, 0.0f, 0.0f };
	//	break;
	//case 2:
	//	// �M�~�b�N2: ��) �L�����N�^�[�𒵂˕Ԃ�
	//	Velocity.y = 10.0f; // ������̑��x��t�^
	//	break;

	//case 3:
	//	// �M�~�b�N3: ��) ����̃A�N�V�����𔭓�����
	//	/*TriggerSpecialAction();*/
	//	break;

	//default:
	//	// ���̃u���b�NID�▢��`�̓���
	//	break;
	//}
}

void Character::UpdateVerticalVelocity(float elapsedTime)
{
	//�d�͏���
	Velocity.y += gravity * elapsedTime;
}

void Character::UpdateHorizonVelocity(float elapsedTime)
{
	////XZ���ʂ̑��͂���������
	float Length = sqrtf(Velocity.x * Velocity.x + Velocity.z * Velocity.z);
	if (Length > 0.0f)
	{
		//���C��
		float Friction = this->friction * elapsedTime;

		//�󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!IsGround()) // IsInAir�t���O��p����
		{
			Friction *= airControl; // �󒆂ł͖��C�͂�30%�Ɍ���
		}
			


		//���C�ɂ�������̌���
		if (Length > Friction)
		{
			 //���C��K�p���đ��x������������
			float decelerationFactor = (Length - Friction) / Length;
			Velocity.x *= decelerationFactor;
			Velocity.z *= decelerationFactor;
		}
		//�������̑��͂����C�ȉ��ɂȂ����Ƃ����͂𖳌���
		else
		{
			Velocity.x = 0.0f;
			Velocity.z = 0.0f;
		}
	}
	// �ő呬�x�����̏ꍇ�̉�������
	float moveVecLength = sqrtf(MoveVecX * MoveVecX + MoveVecZ * MoveVecZ);
	if (moveVecLength > 0.0f && Length <= MaxMoveSpeed)
	{
		// �����͂�K�p
		float acceleration = this->acceleration * elapsedTime;
		Velocity.x += MoveVecX * acceleration;
		Velocity.z += MoveVecZ * acceleration;

		// �ő呬�x����
		float newLength = sqrtf(Velocity.x * Velocity.x + Velocity.z * Velocity.z);
		if (newLength > MaxMoveSpeed)
		{
			// ���K�����đ��x�𐧌�
			float normalizationFactor = MaxMoveSpeed / newLength;
			Velocity.x *= normalizationFactor;
			Velocity.z *= normalizationFactor;
		}
	}

	// �ړ��x�N�g�������Z�b�g
	MoveVecX = 0.0f;
	MoveVecZ = 0.0f;
}


void Character::UpdateVerticalMove(float elapsedTime)
{
	//�O�̂��
	{
		////�ړ�����
		//position.y += Velocity.y * elapsedTime;

		//if (position.y < 0.0f)
		//{
		//	position.y = 0;
		//	Velocity.y = 0;

		//	if (!IsGround())
		//	{
		//		OnLanding();
		//	}
		//	isGround = true;
		//}
		//else
		//{
		//	isGround = false;
		//}
	}
	//���Ȃ�
	{
	//	float my = Velocity.y * elapsedTime;

	//	if (my < 1.0f)
	//	{
	//		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
	//		DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y;

	//			if (!isGround)
	//			{
	//				OnLanding();
	//			}
	//			isGround = true;
	//			Velocity.y = 0.0f;
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//	else if (my > 0.0f)
	//	{
	//		position.y += my;
	//		isGround = false;
	//	}
	}

	int HitBlock = 0;
	float my = Velocity.y * elapsedTime;

	bool BackframeisGround = isGround;
	isGround = false;

	if (my < 1.0f) // �������̈ړ��i�����j
	{
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset * 0.2f, position.z };
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		DirectX::XMFLOAT3 hitPosition;
		DirectX::XMFLOAT3 hitNormal;
		DirectX::XMFLOAT3 BlockAngle;

		if (BackframeisGround)
		{
			end.y += my * 0.3f;
		}

		if (Stage::Instance().UnifiedRayCast(start, end, hitPosition, hitNormal, HitBlock,true))
		{
			switch (HitBlock)
			{
			case 0:
			case 1:
			case 2:
				position.y = hitPosition.y;

				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				break;
			case 3:
				position.y = hitPosition.y;
				isGround = true;
				Jump(16);
				break;
			case 4:
				break;
			}
			
		}
		else
		{
			position.y += my;
			isGround = false;
		}
	}


	//{
	//	if (my < 1.0f) // �������̈ړ��i�����j
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset/2, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y;

	//			if (!isGround)
	//			{
	//				OnLanding();
	//			}
	//			isGround = true;
	//			Velocity.y = 0.0f;
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//	if (my < 1.0f) // �������̈ړ��i�����j
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;
	//		if (Stage::Instance().BlockRayCast(start, end, hitPosition, hitNormal, HitBlock))
	//		{
	//			switch (HitBlock)
	//			{
	//			case 1:
	//			case 2:
	//				position.y = hitPosition.y;

	//				if (!isGround)
	//				{
	//					OnLanding();
	//				}
	//				isGround = true;
	//				Velocity.y = 0.0f;
	//				break;
	//			case 3:
	//				position.y = hitPosition.y;
	//				isGround = true;
	//				Jump(16);
	//				break;
	//			}

	//		}
	//	}
	//	else if (my > 0.0f) // ������̈ړ��i�W�����v�j
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + height - stepOffset, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + height + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y - height - my; // �����V��ɐڐG�����ꍇ�A���̍���������

	//			isGround = false;
	//			Velocity.y = 0.0f; // ������̑��x���[���ɂ���
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//}



}

void Character::UpdateHorizonMove(float elapsedTime)
{
	//���������̈ړ�����
	//position.x += Velocity.x * elapsedTime;
	//position.z += Velocity.z * elapsedTime;
	float moveX = Velocity.x * elapsedTime;
	float moveZ = Velocity.z * elapsedTime;
	float moveLength = sqrtf(moveX * moveX + moveZ * moveZ);
	//������i�d���j
	{
		//float moveX = Velocity.x * elapsedTime;
		//float moveZ = Velocity.z * elapsedTime;
		//float moveLength = sqrtf(moveX * moveX + moveZ * moveZ);
		//// �����̃��C
		//const DirectX::XMFLOAT3 sFeet = { position.x, position.y + 0.3f, position.z };
		//const DirectX::XMFLOAT3 eFeet = { position.x + moveX * 3, position.y + 0.3f, position.z + moveZ * 3 };

		//// ���t�߂̃��C
		//const DirectX::XMFLOAT3 sHead = { position.x, position.y + height - 0.3f, position.z };
		//const DirectX::XMFLOAT3 eHead = { position.x + moveX * 3, position.y + height - 0.3f, position.z + moveZ * 3 };
		////�@���x�N�g���ƌ�_�̕ۑ���
		//DirectX::XMFLOAT3 pFeet, nFeet;
		//DirectX::XMFLOAT3 pHead, nHead;


		//bool shouldRayCast = (position.x != Previousposition.x || position.z != Previousposition.z);
		//Previousposition = position;

		////���Ƒ����ǂ��炪��������������ׂ�
		//bool hitFeet = Stage::Instance().RayCast(sFeet, eFeet, pFeet, nFeet);
		//bool hitHead = Stage::Instance().RayCast(sHead, eHead, pHead, nHead);


		//if (hitFeet || hitHead)
		//{
		//	// �����܂��͓����q�b�g�����ꍇ�A�Փ˓_�Ɩ@�����擾
		//	DirectX::XMFLOAT3 p = hitFeet ? pFeet : pHead;
		//	DirectX::XMFLOAT3 n = hitFeet ? nFeet : nHead;

		//	// �Փ˓_�ƌ��݈ʒu�̍������v�Z
		//	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
		//	DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eFeet);
		//	DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

		//	DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
		//	DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

		//	float a = DirectX::XMVectorGetX(A) + 0.5f; // �߂荞�݂��L�����
		//	DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
		//	DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

		//	DirectX::XMFLOAT3 q;
		//	DirectX::XMStoreFloat3(&q, Q);

		//	// �Փ˓_�ōēx���C�L���X�g���s���ďC�����ꂽ�ʒu���v�Z
		//	if (Stage::Instance().RayCast(sFeet, q, pFeet, nFeet))
		//	{
		//		DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
		//		DirectX::XMFLOAT3 correctedPositionFloat3;
		//		DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

		//		position.x = correctedPositionFloat3.x;
		//		position.z = correctedPositionFloat3.z;
		//	}
		//	else
		//	{
		//		position.x = q.x;
		//		position.z = q.z;
		//	}
		//}
		//else
		//{
		//	position.x += moveX;
		//	position.z += moveZ;
		//}

		//float vx = moveX / moveLength;
		//float vz = moveZ / moveLength;
		//angle.y = atan2f(vx, vz);
	}
	// �����̃��C
	{
		const DirectX::XMFLOAT3 sFeet = { position.x, position.y + stepOffset, position.z };
		const DirectX::XMFLOAT3 eFeet = { position.x + moveX * 2, position.y + stepOffset, position.z + moveZ * 2 };

		// �@���x�N�g���ƌ�_�̕ۑ���
		DirectX::XMFLOAT3 pFeet, nFeet;
		int HitBlock = 0;
		//bool shouldRayCast = (position.x != Previousposition.x || position.z != Previousposition.z);

		// �����̓����蔻�肾���s��
		bool hitFeet = false;
		bool hitBlockFeet = false;
		hitFeet = Stage::Instance().UnifiedRayCast(sFeet, eFeet, pFeet, nFeet, HitBlock,true);

		if (hitFeet)
		{
			Velocity.x = 0;
			Velocity.z = 0;
			 //�������q�b�g�����ꍇ�A�Փ˓_�Ɩ@�����擾
			DirectX::XMFLOAT3 p = pFeet;
			DirectX::XMFLOAT3 n = nFeet;

			 //�Փ˓_�ƌ��݈ʒu�̍������v�Z
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eFeet);
			DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

			DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
			DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

			float a = DirectX::XMVectorGetX(A) - 0.09f; // �߂荞�݂��L�����
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
			DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

			DirectX::XMFLOAT3 q;
			DirectX::XMStoreFloat3(&q, Q);

			 //�Փ˓_�ōēx���C�L���X�g���s���ďC�����ꂽ�ʒu���v�Z
			if (Stage::Instance().UnifiedRayCast(sFeet, q, pFeet, nFeet, HitBlock,true))
			{
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
				DirectX::XMFLOAT3 correctedPositionFloat3;
				DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

				position.x = correctedPositionFloat3.x;
				position.z = correctedPositionFloat3.z;
			}
			else
			{
				position.x = q.x;
				position.z = q.z;
			}
			if (Stage::Instance().RayCast(sFeet, q, pFeet, nFeet))
			{
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
				DirectX::XMFLOAT3 correctedPositionFloat3;
				DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

				position.x = correctedPositionFloat3.x;
				position.z = correctedPositionFloat3.z;
			}
			else
			{
				position.x = q.x;
				position.z = q.z;
			}
		}
		else
		{
			position.x += moveX;
			position.z += moveZ;
		}

	}
}



void Character::Jump(float speed)
{
	Velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime)
{
	//�����̑��͍X�V
	UpdateVerticalVelocity(elapsedTime);
	//�����̑��͍X�V
	 UpdateHorizonVelocity(elapsedTime);
	//�����̈ړ��X�V
	UpdateVerticalMove(elapsedTime);
	//�����̈ړ��X�V
	UpdateHorizonMove(elapsedTime);

	{

		//Velocity.y += gravity * elapsedTime;

		//position.y += Velocity.y * elapsedTime;

		/*if (position.y < 0.0f)
		{
			position.y = 0;
			Velocity.y = 0;

			if (!IsGround())
			{
				OnLanding();
			}
			isGround = true;
		}
		else
		{
			isGround = false;
		}*/
	}

	UpdateTransform();
}

void Character::Updateinvincibletimer(float elapsedTime)
{
	if (invincibletimer > 0.0f)
	{
		invincibletimer -= elapsedTime;
	}
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;
	
	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//--- �K�^���ɑΉ������� ---
	
		// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * vx) + (frontZ * vz);	//���ρF�t�����g���

	// ���ϒl��-1.0�`1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;	//�␳�l
	//rot = 1.0f - dot;	//ImGui�ŕ\�����邽�߂Ƀ����o�[�ϐ��Ƃ���

	float reotationSpeed = speed * (1.0f - fabsf(dot));
	if (rot > speed) rot = speed;	//��]���x�����Arot���傫���ꍇ�́A��]���x���g��

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontZ * vx) - (frontX * vz);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	//���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		//angle.y -= speed;
		angle.y -= rot;
	}
	else
	{
		//angle.y += speed;
		angle.y += rot;
	}
	
}

void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	//speed *= elapsedTime;
	//position.x += vx * speed;
	//position.z += vz * speed;

	MoveVecX = vx; // vx�����̈ړ�����
	MoveVecZ = vz; // vz�����̈ړ�����

	MaxMoveSpeed = speed; // �ő�ړ����x��ݒ�
}