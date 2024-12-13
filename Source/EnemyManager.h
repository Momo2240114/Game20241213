#pragma once

#include <vector>
#include <set>
#include "Enemy.h"

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	// �B��̃C���X�^���X�擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//�G�l�~�[�o�^
	void Register(Enemy* enemy);

	//�G�l�~�[�S�폜
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
