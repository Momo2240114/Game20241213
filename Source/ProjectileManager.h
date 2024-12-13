#pragma once
#include "Projectile.h"
#include <vector>
#include <set>

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);// �`�揈��
	
	void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer);

	//Projectile�o�^
	void Register(Projectile* projectile);

	//Projectile�S�폜
	void Clear();

	int GetprojectileCount()const { return projectiles.size(); }

	void Remove(Projectile* pro);

	Projectile* GetProjectile(int index) {return projectiles.at(index);}

private:
	std::vector<Projectile*> projectiles = {};
	std::set<Projectile*> removes;
};

