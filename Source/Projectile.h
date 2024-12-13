#pragma once

#include "System/ModelRenderer.h"
#include "System/ShapeRenderer.h"

class ProjectileManager;

class Projectile
{
public:
	/*Projectile() {};*/
	Projectile(ProjectileManager* mana);
	virtual ~Projectile() {};

	// çXêVèàóù
	virtual void Update(float elapsedTime) = 0;

	void Destroy();

	// ï`âÊèàóù
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) =0;

	virtual void RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer) ;

	const DirectX::XMFLOAT3& GetPosition() const { return position; };

	const DirectX::XMFLOAT3& GetDirection() const {return direction;}

	const DirectX::XMFLOAT3& GetScale() const { return  scale; };

	const float GetRadius() const { return  radius; };


protected:
	void UpdateTransform();

	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 0,0,0 };
	float radius = 0.5;
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

	ProjectileManager* manager = nullptr;
};

