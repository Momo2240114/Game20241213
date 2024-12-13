#pragma once

#include "System/ModelRenderer.h"
#include "character.h"

// �G�l�~�[
class Enemy : public Character
{
public:
	Enemy() {}
	~Enemy() override {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	void Destroy();

	// �`�揈��
	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) = 0;

};
