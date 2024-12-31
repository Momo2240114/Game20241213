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
