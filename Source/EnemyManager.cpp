#include "EnemyManager.h"
#include "Collision.h"

// 更新処理
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

// 描画処理
void EnemyManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	for (Enemy* enemy : enemies)
	{
		enemy->Render(rc, renderer);
	}
}

//エネミー登録
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

				// 敵iと敵jの衝突チェック
				if (Collision::IntersectVsSCylinder(
					enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
					enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
					outposition)) {
					// 敵iと敵jの衝突チェック
					if (Collision::IntersectVsSCylinder(
						enemies.at(i)->GetPosition(), enemies.at(i)->GetRadius(), enemies.at(i)->GetHeight(),
						enemies.at(j)->GetPosition(), enemies.at(j)->GetRadius(), enemies.at(j)->GetHeight(),
						outposition))
					{
						// i番とj番の位置を取得
						DirectX::XMFLOAT3 posI = enemies.at(i)->GetPosition();
						DirectX::XMFLOAT3 posJ = enemies.at(j)->GetPosition();

						// outpositionを基に修正ベクトルを計算
						DirectX::XMVECTOR vecPosJ = DirectX::XMLoadFloat3(&posJ);
						DirectX::XMVECTOR vecOutPos = DirectX::XMLoadFloat3(&outposition);
						DirectX::XMVECTOR vecDisplacement = DirectX::XMVectorSubtract(vecOutPos, vecPosJ);

						// 反発係数を掛ける
						const float reboundFactor = 1.65f; // 反発係数（1.0より大きくすることで反発が強くなる）
						vecDisplacement = DirectX::XMVectorScale(vecDisplacement, reboundFactor * 0.5f); // 半分ずつに分ける

						// i番とj番の新しい位置を計算
						DirectX::XMVECTOR vecPosI = DirectX::XMLoadFloat3(&posI);
						vecPosI = DirectX::XMVectorSubtract(vecPosI, vecDisplacement); // i番の敵を逆方向に移動
						vecPosJ = DirectX::XMVectorAdd(vecPosJ, vecDisplacement);      // j番の敵を修正方向に移動

						// 新しい位置をXMFLOAT3に戻す
						DirectX::XMStoreFloat3(&posI, vecPosI);
						DirectX::XMStoreFloat3(&posJ, vecPosJ);

						// 敵の位置を更新
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
