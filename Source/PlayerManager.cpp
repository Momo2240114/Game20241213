#include "PlayerManager.h"

void PlayerManager::Update(float elapsedTime)
{
	for (Player* player : players)
	{
		player->Update(elapsedTime);
	}

	for (Player* Pla: removes)
	{
		std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), Pla);
		{
			if (it != players.end())
			{
				players.erase(it);
			}
		}
		delete Pla;
	}
	removes.clear();
}

void PlayerManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Player* player : players)
	{
		player->Render(rc,renderer);
	}
}

void PlayerManager::Register(Player* player)
{
	playerNo++;
	players.emplace_back(player);
}

void PlayerManager::Clear()
{
	for (Player* player : players)
	{
		delete player;
	}
	players.clear();
}

void PlayerManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
}


void PlayerManager::Remove(Player* player)
{
	removes.insert(player);
}

//プレイヤーの再出現の管理用
bool PlayerManager::PopCool(float elapsedTime)
{
	const int PlayerMax = 5;
		if (!players.empty() && players.back()->pupCool())
		{
			moveStateTimer += elapsedTime;
			if (PopTime < moveStateTimer && PopPlayerNum() <= PlayerMax)
			{
				moveStateTimer = 0.0f; // タイマーをリセット
				return true;
			}
		}
		for (Player* player : players)
		{
			if (player->ISGoal())
			{
				Remove(player);
				if (PopPlayerNum() <= PlayerMax)
				{
					moveStateTimer = 0.0f; // タイマーをリセット
					return true;
				}
			}
		}
	return false;
}
