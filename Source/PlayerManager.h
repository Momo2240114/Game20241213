#pragma once

#include <vector>
#include <set>
#include "Player.h"

class PlayerManager
{
private:
	PlayerManager() {};
	~PlayerManager() {};

public:
	// �B��̃C���X�^���X�擾
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//�v���C���[�o�^
	void Register(Player* enemy);

	//�v���C���[�S�폜
	void Clear();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	int GetEnemyCount()const { return static_cast<int>(players.size()); };

	Player* GetEnemy(int index) { return players.at(index); };

	void Remove(Player* player);

private:
	std::vector<Player*>		players;
	std::set<Player*>		removes;
};
