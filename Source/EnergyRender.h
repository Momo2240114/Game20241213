#pragma once

#include "System/ModelRenderer.h"

class EnergyRender
{
public:
	static EnergyRender& Instance() {
		static EnergyRender ins;
		return ins;
	}
	void Init();

	void deInit();

	void SetLockBlock(DirectX::XMFLOAT3 pos, int Ene);

	void AngleSet(DirectX::XMFLOAT3 cameraFront);

	void Render(const RenderContext& rc, ModelRenderer* renderer);
private:
	EnergyRender() {};
	~EnergyRender() {};
	EnergyRender(const EnergyRender* i) {};
	void operator=(const EnergyRender* i) {};

	DirectX::XMFLOAT3 position = {};
	DirectX::XMFLOAT3 angle = {};
	DirectX::XMFLOAT3 scale = {1,1,1};
	
	DirectX::XMFLOAT4X4 Transform = {};
	int Energy = -1;
	Model* EnergyUIMedel[4] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};
};

