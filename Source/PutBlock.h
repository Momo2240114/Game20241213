#pragma once
#include "Player.h"
#include <imgui.h>
#include "Camera.h"
#include "Collision.h"
#include <DirectXMath.h>
#include "System/Graphics.h"
#include "Stage.h"

class PutBlock
{
public:
	static PutBlock& Instance();
	void putBlockOnBlock();
	void SetBlock();
	void Initialize();
	void Finalize();
	void render(const RenderContext& rc, ModelRenderer* renderer);
	void UpdateTransform();
	void Update(float elapsedTime);
	void performRaycast(DirectX::XMFLOAT3& RayState, DirectX::XMFLOAT3& RayEnd);
	

private:
	PutBlock() {};
	~PutBlock() {};

	const int maxType = 6;
	const int minType = 0;

	Model* PutBlockModel = nullptr;
	Model* EraseBlockModel = nullptr;
	Model* PutBlockModel1 = nullptr;
	Model* PutBlockModel2 = nullptr;
	Model* PutBlockModel3 = nullptr;
	Model* PutBlockModel4 = nullptr;
	Model* PutBlockModel5 = nullptr;
	Model* PutBlockModel6 = nullptr;
	DirectX::XMFLOAT3 RayState = { 0,0,0 };
	DirectX::XMFLOAT3 RayEnd = { 0,0,0 };
	DirectX::XMVECTOR WorldPositionFar = {};
	DirectX::XMFLOAT3 Angle = { 0,0,0 };
	DirectX::XMVECTOR ScreenPosition = {};
	DirectX::XMVECTOR WorldPosition = {};
	DirectX::XMFLOAT3 screenPosition = { 0,0,0 };
	DirectX::XMFLOAT3 HitPos = { 0,0,10 };
	DirectX::XMFLOAT3 HitNor = {0,0,0};
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	float mouseX=0;
	float mouseY=0;
	float screenWidth=0;
	float screenHeight=0;
	int Type = 1;
	int HitType = 1;
	float Timer = 0;
	int wheel = 0;
};

