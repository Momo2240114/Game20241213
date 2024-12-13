#include "EnemyManager.h"
#include "Collision.h"

// �X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}


	for (Enemy* ene : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(),ene);
		{
			if (it != enemies.end())
			{
				enemies.erase(it);
			}
		}
		delete ene;
	}
	removes.clear();

	CollisionEnemyVsEnemys();
}

// �`�揈��
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

void EnemyManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Enemy* enemy : enemies)
	{
		enemy->RenderDebugPrimitive(rc, renderer);
	}
}

void EnemyManager::CollisionEnemyVsEnemys()
{
	for (int i = 0; i < GetEnemyCount(); ++i)
	{
		for (int j = 0; j < GetEnemyCount(); ++j)
		{
			if (i != j)
			{
				DirectX::XMFLOAT3 outposition;

				// �Gi�ƓGj�̏Փ˃`�F�b�N
				if (Collision::IntersectVsSCylinder(
					enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
					enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
					outposition)) {
					// �Gi�ƓGj�̏Փ˃`�F�b�N
					if (Collision::IntersectVsSCylinder(
						enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
						enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
						outposition))
					{
						// i�Ԃ�j�Ԃ̈ʒu���擾
						DirectX::XMFLOAT3 posI = enemies.at(i)->GetPosition();
						DirectX::XMFLOAT3 posJ = enemies.at(j)->GetPosition();

						// outposition����ɏC���x�N�g�����v�Z
						DirectX::XMVECTOR vecPosJ = DirectX::XMLoadFloat3(&posJ);
						DirectX::XMVECTOR vecOutPos = DirectX::XMLoadFloat3(&outposition);
						DirectX::XMVECTOR vecDisplacement = DirectX::XMVectorSubtract(vecOutPos, vecPosJ);

						// �����W�����|����
						const float reboundFactor = 1.65f; // �����W���i1.0���傫�����邱�ƂŔ����������Ȃ�j
						vecDisplacement = DirectX::XMVectorScale(vecDisplacement, reboundFactor * 0.5f); // �������ɕ�����

						// i�Ԃ�j�Ԃ̐V�����ʒu���v�Z
						DirectX::XMVECTOR vecPosI = DirectX::XMLoadFloat3(&posI);
						vecPosI = DirectX::XMVectorSubtract(vecPosI, vecDisplacement); // i�Ԃ̓G���t�����Ɉړ�
						vecPosJ = DirectX::XMVectorAdd(vecPosJ, vecDisplacement);      // j�Ԃ̓G���C�������Ɉړ�

						// �V�����ʒu��XMFLOAT3�ɖ߂�
						DirectX::XMStoreFloat3(&posI, vecPosI);
						DirectX::XMStoreFloat3(&posJ, vecPosJ);

						// �G�̈ʒu���X�V
						enemies.at(i)->SetPosition(posI);
						enemies.at(j)->SetPosition(posJ);
					}
					//{
			//	enemies.at(j)->SetPosition(outposition);
			//}
				}
			}
		}
	}
}

void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}
