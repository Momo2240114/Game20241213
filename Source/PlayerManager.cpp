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
		Remove(player);
	}

	playerNo = 0;
	moveStateTimer = 0;
	GoalNum = 0;
	DeadNum = 0;
	HP = 3;
	for (Player* Pla : removes)
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
					GoalNum++;
					if (PopPlayerNum() <= PlayerMax && players.back()->pupCool())
					{
						moveStateTimer = 0.0f; // タイマーをリセット
						return true;
					}
				}
				if (player->IsDead())
				{
					Remove(player);
					DeadNum++;
					HP--;
					if (PopPlayerNum() <= PlayerMax && players.back()->pupCool())
					{
						moveStateTimer = 0.0f; // タイマーをリセット
						return true;
					}
				}
		}
	return false;
}

void PlayerManager::DrawDebugGUI()
{
	for (Player* player : players)
	{
		player->DrawDebugGUI();
	}
}

bool PlayerManager::GameClear()
{
	if (GoalNum > 3) { return true; }
	return false;
}

bool PlayerManager::EndGame()
{
	if (DeadNum > 3) { return true; }
	return false;
}

void PlayerManager::Init()
{
	HP = 3;
}
