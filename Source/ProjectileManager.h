#pragma once
#include "Projectile.h"
#include <vector>
#include <set>

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	// XVˆ—
	void Update(float elapsedTime);

	// •`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer);// •`‰æˆ—
	
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//Projectile“o˜^
	void Register(Projectile* projectile);

	//Projectile‘Síœ
	void Clear();

	int GetprojectileCount()const { return projectiles.size(); }

	void Remove(Projectile* pro);

	Projectile* GetProjectile(int index) {return projectiles.at(index);}

private:
	std::vector<Projectile*> projectiles = {};
	std::set<Projectile*> removes;
};

