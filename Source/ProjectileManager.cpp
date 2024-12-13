#include "ProjectileManager.h"


ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
	Clear();
}

void ProjectileManager::Update(float elapsedTime)
{
	for (Projectile* pro : projectiles)
	{
		pro->Update(elapsedTime);
	}

	for (Projectile* pro : removes)
	{
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(),projectiles.end(), pro);
		{
			if (it != projectiles.end())
			{
				projectiles.erase(it);
			}
		}
		delete pro;
	}
	removes.clear();
}

void ProjectileManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (Projectile* pro : projectiles)
	{
		pro->Render(rc,renderer);
	}
}


void ProjectileManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	for (Projectile* pro : projectiles)
	{
		pro->RenderDebugPrimitive(rc, renderer);
	}
}

void ProjectileManager::Register(Projectile* projectile)
{
	projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
	for (Projectile* pro : projectiles)
	{
		delete pro;
	}
	projectiles.clear();
}

void ProjectileManager::Remove(Projectile* pro)
{
	removes.insert(pro);
}

