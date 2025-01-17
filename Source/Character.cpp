#include "Character.h"
#include "Stage.h"
#include "DirectXMath.h"
#include "KeyPut.h"

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
	
}

void Character::UpdateVerticalVelocity(float elapsedTime)
{
	//�d�͏���
	Velocity.y += gravity * elapsedTime;
}

void Character::UpdateHorizonVelocity(float elapsedTime)
{
	if (WarpCool > 0)WarpCool -= elapsedTime;
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
			
		////���C�ɂ�������̌���
		if (Length > Friction)
		{
			// //���C��K�p���đ��x������������
			//float decelerationFactor = (Length - Friction) / Length;
			//Velocity.x *= decelerationFactor;
			//Velocity.z *= decelerationFactor;
		}
		//�������̑��͂����C�ȉ��ɂȂ����Ƃ����͂𖳌���
		else
		{
			//Velocity.x = 0.0f;
			//Velocity.z = 0.0f;
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
	//MoveVecX = 0.0f;
	//MoveVecZ = 0.0f;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
	float Blocksize = Stage::Instance().GetBlockSize();
	int mapX = Stage::Instance().GetMapXsize();
	int mapZ = Stage::Instance().GetMapZsize();
	DirectX::XMFLOAT3 Bscale = Stage::Instance().GetBlockscale();
	int Putx = 0;
	int	Putz = 0;
	if ((int)(position.x + 0.5f) > 0)
	{
		Putx = static_cast<int>((position.x + 0.5f) / (Blocksize * Bscale.x));
	}
	else if ((int)(position.x - 0.5f) < 0)
	{
		Putx = static_cast<int>((position.x - 0.5f) / (Blocksize * Bscale.x));
	}
	if ((int)(position.z + 0.5f) > 0)
	{
		Putz = static_cast<int>((position.z + 0.5f) / (Blocksize * Bscale.z));
	}
	else if ((int)(position.z) - 0.5f < 0)
	{
		Putz = static_cast<int>((position.z - 0.5f) / (Blocksize * Bscale.z));
	}

	int HitBlock = -1;//���������u���b�N�̔��ʗp

	float my = Velocity.y * elapsedTime;//�d�͂̋���

	bool BackframeisGround = isGround;   //�O�̃t���[���ɒn�ʂɂ������𔻕�

	isGround = false;

	if (my < 1.0f) // �������̈ړ��i�����j
	{
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		DirectX::XMFLOAT3 hitPosition;
		DirectX::XMFLOAT3 hitNormal;

		if (BackframeisGround) //�O�̃t���[���ɒn�ʂɂ����
		{
			end.y += my * 0.3f;//���C�L���X�g�̏I�_��L�΂��č⓹�������悤�ɂ���
		}



		if (Stage::Instance().UnifiedRayCast(start, end, hitPosition, hitNormal, HitBlockAngle, HitBlock, true, true))
		{
			bool isNewBlock = true;
			// �u���b�N���W�̌v�Z
			int Px = static_cast<int>(round(hitPosition.x / (Bscale.x * Blocksize)));
			int Py = static_cast<int>(round(hitPosition.y / (Bscale.y * Blocksize)));
			int Pz = static_cast<int>(round(hitPosition.z / (Bscale.z * Blocksize)));

			// �v�Z�덷���l��������r
			const float epsilon = 0.02f; // �덷���e�͈�
			if (std::abs(LastHitPos.x - Px) < epsilon &&
				std::abs(LastHitPos.y - Py) < epsilon &&
				std::abs(LastHitPos.z - Pz) < epsilon) {
				isNewBlock = false;
			}

			// �Ō�ɓ��񂾈ʒu���X�V
			LastHitPos = { static_cast<float>(Px), static_cast<float>(Py), static_cast<float>(Pz) };

			//���������u���b�N�ɉ����ď�����ς���
			switch (HitBlock)
			{
			case 0:
			case 1:
			case 2:
			case 102:
			case 103:
			case 107:
				position.y = hitPosition.y;
				StopState = 0;
				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				OnMovingFloorTime = 0;
				break;
			case 3://�W�����v��

				position.y = hitPosition.y;

				isGround = true;
				if (Stage::Instance().BlockEnergy(hitPosition))
				{
					Jump(16);
				}
				if (isNewBlock)
				{
					Stage::Instance().DamageBlock(hitPosition);
				}
				OnMovingFloorTime = 0;
				break;
			case 4:

				position.y = hitPosition.y;
				StopState = 0;
				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				OnMovingFloorTime++;
				//�u���b�N�̒[�ō쓮����̂�h��
				if (OnMovingFloorTime > 4)
				{
					if (Stage::Instance().BlockEnergy(hitPosition))
					{
						HitAngleCheck(HitBlockAngle.y);
					}
				}
				if (isNewBlock)
				{
					Stage::Instance().DamageBlock(hitPosition);
				}
				break;
			case 5:

				StopState = 0;
				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				if (Stage::Instance().BlockEnergy(hitPosition))
				{
					Jump(15);
					//���������u���b�N��Angle.y�ɉ�����Velocity.x,z��ς���
					HitAngleCheck(HitBlockAngle.y);
				}
				if (isNewBlock)
				{
					Stage::Instance().DamageBlock(hitPosition);
				}
				break;

			case 6:

				if (Stage::Instance().BlockEnergy(hitPosition))
				{
					switch (StopState)
					{
					case 0: // ��~���ɐG�ꂽ�u�ԁi���񏈗��j
						Stage::Instance().DamageBlock(hitPosition);
						// �����~���̑��x�ۑ�
						OldVelocity = Velocity;
						Velocity = { 0.0f, 0.0f, 0.0f }; // ��~��Ԃɂ���
						OnMovingFloorTime = 0.0f; // �^�C�}�[�����Z�b�g

						// ���S�ւ̃^�[�Q�b�g���W��ݒ�i2�̔{���ɕ␳�j
						TargetPosition.x = static_cast<float>(round(position.x / 2.0f) * 2);
						TargetPosition.z = static_cast<float>(round(position.z / 2.0f) * 2);

						StopState = 1; // ���̏�Ԃֈڍs
						break;

					case 1: // ���X�ɒ�~���̒��S�ֈړ�
						if (std::abs(position.x - TargetPosition.x) > 0.1f ||
							std::abs(position.z - TargetPosition.z) > 0.1f)
						{
							// �������^�[�Q�b�g���W�Ɍ������Ĉړ�
							position.x += (TargetPosition.x - position.x) * 0.05f;
							position.z += (TargetPosition.z - position.z) * 0.05f;

							// �������̂��߁A�ᑬ�őO�i
							Velocity.x = (TargetPosition.x - position.x) * 0.1f;
							Velocity.z = (TargetPosition.z - position.z) * 0.1f;
						}
						else
						{
							// �ړI�n�ɓ��B������ʒu���Œ�
							position.x = TargetPosition.x;
							position.z = TargetPosition.z;
							Velocity = { 0.0f, 0.0f, 0.0f }; // �ēx��~
							StopState = 2; // Enter�L�[�҂��ֈڍs
						}
						break;

					case 2: // Enter�L�[�Ⴕ���͈�莞�ԑ҂�
						OnMovingFloorTime += elapsedTime;
						if (KeyPressed(VK_RETURN) || OnMovingFloorTime > 3) { // Enter�L�[�������ꂽ��ĊJ
							StopState = 3;
						}
						break;

					case 3: // ��~����
						Velocity = OldVelocity; // �ۑ����Ă��������x�𕜌�
						StopState = 4; // ������Ԃֈڍs
						break;

					case 4: // �������
						// ������A�ēx��~�������������Ȃ��悤�ɑҋ@
						break;
					}
				}
				break;
			case 7:
				if (isNewBlock)
				{
					Stage::Instance().DamageBlock(hitPosition);
				}
				if (Stage::Instance().BlockEnergy(hitPosition))
				{
					accel = 3;
				}
				break;
			case 104:
				switch (StopState)
				{
				case 0:
					Stage::Instance().BlockChange();
					StopState++;
					break;
				case 1:

					break;
				}
				break;
			case 105:
				position.y = hitPosition.y;
				StopState = 0;
				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				OnMovingFloorTime += elapsedTime;
				if (OnMovingFloorTime > 1)
				{
					IsGoal = true;
					Velocity = { 0.0f, 0.0f, 0.0f }; //��~
				}
				break;
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
				if (Stage::Instance().BlockEnergy(hitPosition))
				{
					if (WarpCool <= 0)
					{

						switch (StopState)
						{
						case 0: // ��~���ɐG�ꂽ�u�ԁi���񏈗��j
							Stage::Instance().DamageBlock(hitPosition);
							// �����~���̑��x�ۑ�
							OldVelocity = Velocity;
							Velocity = { 0.0f, 0.0f, 0.0f }; // ��~��Ԃɂ���
							OnMovingFloorTime = 0.0f; // �^�C�}�[�����Z�b�g

							// ���S�ւ̃^�[�Q�b�g���W��ݒ�i2�̔{���ɕ␳�j
							TargetPosition.x = static_cast<float>(round(position.x / 2.0f) * 2);
							TargetPosition.z = static_cast<float>(round(position.z / 2.0f) * 2);

							StopState = 1; // ���̏�Ԃֈڍs
							break;

						case 1: // ���X�ɒ�~���̒��S�ֈړ�
							if (std::abs(position.x - TargetPosition.x) > 0.1f ||
								std::abs(position.z - TargetPosition.z) > 0.1f)
							{
								// �������^�[�Q�b�g���W�Ɍ������Ĉړ�
								position.x += (TargetPosition.x - position.x) * 0.05f;
								position.z += (TargetPosition.z - position.z) * 0.05f;

								// �������̂��߁A�ᑬ�őO�i
								Velocity.x = (TargetPosition.x - position.x) * 0.1f;
								Velocity.z = (TargetPosition.z - position.z) * 0.1f;
							}
							else
							{
								// �ړI�n�ɓ��B������ʒu���Œ�
								position.x = TargetPosition.x;
								position.z = TargetPosition.z;
								Velocity = { 0.0f, 0.0f, 0.0f }; // �ēx��~
								StopState = 2; // Enter�L�[�҂��ֈڍs
							}
							break;

						case 2: // Enter�L�[�Ⴕ���͈�莞�ԑ҂�
							OnMovingFloorTime += elapsedTime;
							angle.y += DirectX::XMConvertToRadians(720) * elapsedTime;
							if (OnMovingFloorTime > 2) { // Enter�L�[�������ꂽ��ĊJ
								StopState = 3;
							}
							break;

						case 3: // ��~����
							//�����������[�v���ɉ����đ΂ɂȂ郏�[�v���Ƀ��[�v
								// ���[�v���T��
							DirectX::XMFLOAT3 targetPos = Stage::Instance().SearchPairPoint(HitBlock, position);

							// �ʒu�����[�v��ɐݒ�
							position = targetPos;
							Velocity.y = 0.0f;
							angle.y = ((int)(angle.y + 45) / 90) * 90;
							StopState = 4; // ������Ԃֈڍs
							break;

						case 4: // �������
							// OldVelocity �̕����𒲐�����

							// OldVelocity �̕����𒲐����ĕ���
							if (OldVelocity.x != 0.0f || OldVelocity.z != 0.0f) {
								if (std::abs(OldVelocity.x) >= std::abs(OldVelocity.z)) {
									// x �������̑��x���傫���ꍇ
									OldVelocity.z = 0.0f; // z ���̑��x�� 0 �ɂ���
									OldVelocity.x = (OldVelocity.x > 0.0f ? moveSpeed : -moveSpeed); // x ���̕�����ێ�
								}
								else {
									// z �������̑��x���傫���ꍇ
									OldVelocity.x = 0.0f; // x ���̑��x�� 0 �ɂ���
									OldVelocity.z = (OldVelocity.z > 0.0f ? moveSpeed : -moveSpeed); // z ���̕�����ێ�
								}
							}
							Velocity = OldVelocity; // �ۑ����Ă��������x�𕜌�
							WarpCool = 15; // �ēx��~�������������Ȃ��悤�ɑҋ@
							Stage::Instance().DamageBlock(hitPosition);
							break;
						}
					}
				}
				break;
			case 106:
				IsLive = true;
				break;
			}

		}
		else
		{
			accel -= 0.02f;
			if (accel < 1.0f)accel = 1.0f;
			OnMovingFloorTime = 0;
			StopState = 0;
			position.y += my;
			isGround = false;
		}
	}
	if (my > 0.0f) // ������̈ړ��i�W�����v�j
	{
		// ������Ń��C�𔭎�
		DirectX::XMFLOAT3 start = { position.x, position.y + height - stepOffset, position.z }; // ��+�I�t�Z�b�g
		DirectX::XMFLOAT3 end = { position.x, position.y + my + height, position.z }; // �W�����v��̑����ʒu

		DirectX::XMFLOAT3 hitPosition;
		DirectX::XMFLOAT3 hitNormal;

		if (Stage::Instance().UnifiedRayCast(start, end, hitPosition, hitNormal, HitBlockAngle, HitBlock, true, true))
		{
			// �V��ɏՓ˂����ꍇ�A������Y���W��V��̏Փˈʒu����L�����N�^�[�̍���������
			position.y = hitPosition.y - height;

			isGround = false;
			Velocity.y = 0.0f; // ������̑��x���[���ɂ���
		}
		else
		{
			// �Փ˂��Ȃ��ꍇ�͒ʏ�ʂ�Y���W�����Z
			position.y += my;
			isGround = false;
		}
	}
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

		DirectX::XMFLOAT3 sHead = { position.x, position.y + height, position.z }; // ��+�I�t�Z�b�g
		DirectX::XMFLOAT3 eHead = { position.x + moveX * 2, position.y + height, position.z + moveZ * 2 }; // �W�����v��̑����ʒu

		// �@���x�N�g���ƌ�_�̕ۑ���
		DirectX::XMFLOAT3 pFeet, nFeet;
		DirectX::XMFLOAT3 pHead, nHead;

		int HitBlock = 0;
		//bool shouldRayCast = (position.x != Previousposition.x || position.z != Previousposition.z);

		// �����̓����蔻�肾���s��
		bool hitFeet = false;
		bool hitHead = false;
		bool hitBlockFeet = false;
		hitFeet = Stage::Instance().UnifiedRayCast(sFeet, eFeet, pFeet, nFeet, HitBlockAngle, HitBlock,true,true);
		hitHead = Stage::Instance().UnifiedRayCast(sHead, eHead, pHead, nHead, HitBlockAngle, HitBlock, true, true);
		if (hitFeet)
		{
			if (HitBlock == 106)
			{
				IsLive = true;
			}


			Velocity.x = 0;
			Velocity.z = 0;
			 //�������q�b�g�����ꍇ�A�Փ˓_�Ɩ@�����擾
			DirectX::XMFLOAT3 p = pFeet;
			DirectX::XMFLOAT3 n = nFeet;
			DirectX::XMFLOAT3 an;

			 //�Փ˓_�ƌ��݈ʒu�̍������v�Z
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eFeet);
			DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

			DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
			DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

			float a = DirectX::XMVectorGetX(A) - 0.2f; // �߂荞�݂��L�����
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
			DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

			DirectX::XMFLOAT3 q;
			DirectX::XMStoreFloat3(&q, Q);

			 //�Փ˓_�ōēx���C�L���X�g���s���ďC�����ꂽ�ʒu���v�Z
			if (Stage::Instance().UnifiedRayCast(sFeet, q, pFeet, nFeet, an, HitBlock,true))
			{

				Velocity.x = 0;
				Velocity.z = 0;
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
		else if(hitHead)
		{
			if (HitBlock == 106 || HitBlock == 107)
			{
				IsLive = true;
			}


			Velocity.x = 0;
			Velocity.z = 0;
			//�������q�b�g�����ꍇ�A�Փ˓_�Ɩ@�����擾
			DirectX::XMFLOAT3 p = pHead;
			DirectX::XMFLOAT3 n = nHead;
			DirectX::XMFLOAT3 an;

			//�Փ˓_�ƌ��݈ʒu�̍������v�Z
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eHead);
			DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

			DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
			DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

			float a = DirectX::XMVectorGetX(A) - 0.09f; // �߂荞�݂��L�����
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
			DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

			DirectX::XMFLOAT3 q;
			DirectX::XMStoreFloat3(&q, Q);

			//�Փ˓_�ōēx���C�L���X�g���s���ďC�����ꂽ�ʒu���v�Z
			if (Stage::Instance().UnifiedRayCast(sHead, q, pHead, nHead, an, HitBlock, true))
			{
				Velocity.x = 0;
				Velocity.z = 0;
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pHead), DirectX::XMVectorScale(N, 0.05f));
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
			if (Stage::Instance().RayCast(sHead, q, pHead, nHead))
			{
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pHead), DirectX::XMVectorScale(N, 0.05f));
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

void Character::HitAngleCheck(float HitAngle)
{
	constexpr float anglepattern = DirectX::XMConvertToRadians(90.0f);//�u���b�N�̉�]��90�x���Ƃ���ׂ�悤

	//���������u���b�N��Angle.y�ɉ�����Velocity.x,z��ς���
	if (std::abs(HitBlockAngle.y - 0.0f) < 0.01f) {
		OnMovingFloorTime = 0;
		angle.y = anglepattern * 3;
		Velocity.x = -moveSpeed;
		Velocity.z = 0;
		OnMovingFloorTime = 0;
		// ���߂�2�̔{����z�ʒu��␳
		position.z = static_cast<float>(round(position.z / 2.0f) * 2);
	}
	else if (std::abs(HitBlockAngle.y - anglepattern) < 0.01f) {
		angle.y = 0;
		Velocity.x = 0;
		Velocity.z = moveSpeed;
		// ���߂�2�̔{����x�ʒu��␳
		position.x = static_cast<float>(round(position.x / 2.0f) * 2);


	}
	else if (std::abs(HitBlockAngle.y - anglepattern * 2) < 0.01f) {
		angle.y = anglepattern;
		Velocity.x = moveSpeed;
		Velocity.z = 0;
		OnMovingFloorTime = 0;
		// ���߂�2�̔{����z�ʒu��␳
		position.z = static_cast<float>(round(position.z / 2.0f) * 2);
	}
	else if (std::abs(HitBlockAngle.y - anglepattern * 3) < 0.01f) {
		angle.y = anglepattern * 2;
		Velocity.x = 0;
		Velocity.z = -moveSpeed;
		// ���߂�2�̔{����x�ʒu��␳
		position.x = static_cast<float>(round(position.x / 2.0f) * 2);
		OnMovingFloorTime = 0;
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