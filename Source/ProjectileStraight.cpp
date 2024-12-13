#include "ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");

	scale.x = scale.y = scale.z = 3.5f;
}

ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
	lifetimer -= elapsedTime;
	if (lifetimer < 0)
	{
		Destroy();
	}
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	UpdateTransform();
	model->UpdateTransform();
}

void ProjectileStraight::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& dir, const DirectX::XMFLOAT3& pos)
{
	this->direction = dir;
	this->position = pos;
}


