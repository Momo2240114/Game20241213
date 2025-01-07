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
	// 唯一のインスタンス取得
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	//プレイヤー登録
	void Register(Player* enemy);

	//プレイヤー全削除
	void Clear();

	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	int GetEnemyCount()const { return static_cast<int>(players.size()); };

	Player* GetEnemy(int index) { return players.at(index); };

	void Remove(Player* player);

	bool PopCool(float elapsedTime);

	int PopPlayerNum() { return playerNo; }

	//デバッグ用GUI描画
	void DrawDebugGUI();


	bool EndGame() {
		if (GoalNum + DeadNum >= PlayerMax)return true;
		return false;
	}
private:
	std::vector<Player*>		players;
	std::set<Player*>		removes;

	int playerNo = 0;
	float moveStateTimer = 0;
	const int PopTime = 10;

	const int PlayerMax = 15;
	int GoalNum = 0;
	int DeadNum = 0;
};

