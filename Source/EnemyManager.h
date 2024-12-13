#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

// エネミーマネージャー
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	// 唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//エネミー登録
	void Register(Enemy* enemy);

	//エネミー全削除
	void Clear();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	int GetEnemyCount()const { return static_cast<int>(enemies.size()); };

	Enemy* GetEnemy(int index) { return enemies.at(index); };

	void CollisionEnemyVsEnemys();

	void Remove(Enemy* enemy);

private:
	std::vector<Enemy*>		enemies;
	std::set<Enemy*>		removes;
};
