#pragma once

#include "System/Model.h"
#include "Projectile.h"

class ProjectileStraight: public Projectile
{
public:
	/*ProjectileStraight();*/
	ProjectileStraight(ProjectileManager* manager);
	~ProjectileStraight()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer)override;

	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
	Model* model = nullptr;
	float speed = 10.0f;
	float lifetimer = 3.0f;
};

