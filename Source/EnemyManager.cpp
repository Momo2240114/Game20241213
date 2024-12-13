#include "EnemyManager.h"
#include "Collision.h"

// XVˆ—
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

// •`‰æˆ—
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//ƒGƒlƒ~[“o˜^
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

				// “Gi‚Æ“Gj‚ÌÕ“Ëƒ`ƒFƒbƒN
				if (Collision::IntersectVsSCylinder(
					enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
					enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
					outposition)) {
					// “Gi‚Æ“Gj‚ÌÕ“Ëƒ`ƒFƒbƒN
					if (Collision::IntersectVsSCylinder(
						enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
						enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
						outposition))
					{
						// i”Ô‚Æj”Ô‚ÌˆÊ’u‚ðŽæ“¾
						DirectX::XMFLOAT3 posI = enemies.at(i)->GetPosition();
						DirectX::XMFLOAT3 posJ = enemies.at(j)->GetPosition();

						// outposition‚ðŠî‚ÉC³ƒxƒNƒgƒ‹‚ðŒvŽZ
						DirectX::XMVECTOR vecPosJ = DirectX::XMLoadFloat3(&posJ);
						DirectX::XMVECTOR vecOutPos = DirectX::XMLoadFloat3(&outposition);
						DirectX::XMVECTOR vecDisplacement = DirectX::XMVectorSubtract(vecOutPos, vecPosJ);

						// ”½”­ŒW”‚ðŠ|‚¯‚é
						const float reboundFactor = 1.65f; // ”½”­ŒW”i1.0‚æ‚è‘å‚«‚­‚·‚é‚±‚Æ‚Å”½”­‚ª‹­‚­‚È‚éj
						vecDisplacement = DirectX::XMVectorScale(vecDisplacement, reboundFactor * 0.5f); // ”¼•ª‚¸‚Â‚É•ª‚¯‚é

						// i”Ô‚Æj”Ô‚ÌV‚µ‚¢ˆÊ’u‚ðŒvŽZ
						DirectX::XMVECTOR vecPosI = DirectX::XMLoadFloat3(&posI);
						vecPosI = DirectX::XMVectorSubtract(vecPosI, vecDisplacement); // i”Ô‚Ì“G‚ð‹t•ûŒü‚ÉˆÚ“®
						vecPosJ = DirectX::XMVectorAdd(vecPosJ, vecDisplacement);      // j”Ô‚Ì“G‚ðC³•ûŒü‚ÉˆÚ“®

						// V‚µ‚¢ˆÊ’u‚ðXMFLOAT3‚É–ß‚·
						DirectX::XMStoreFloat3(&posI, vecPosI);
						DirectX::XMStoreFloat3(&posJ, vecPosJ);

						// “G‚ÌˆÊ’u‚ðXV
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
