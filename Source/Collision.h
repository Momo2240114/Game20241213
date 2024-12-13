#pragma once

#include <DirectXMath.h>
#include "System/ModelRenderer.h"
#include "System/Model.h"

class Collision
{
public:
	static bool IntersectVsShaere(
		const DirectX::XMFLOAT3& pA,
		float RadiusA,
		const DirectX::XMFLOAT3& pB,
		float RadiusB,
		DirectX::XMFLOAT3& outPB
	);
	static bool IntersectVsSCylinder(
		const DirectX::XMFLOAT3& pA,
		float RadA,
		float HeiA,
		const DirectX::XMFLOAT3& pB,
		float RadB,
		float HeiB,
		DirectX::XMFLOAT3& outPB
	);
	static bool InterctSpherVSCylinder(
		const DirectX::XMFLOAT3& pSp,
		float RadiusSp,
		const DirectX::XMFLOAT3& pCy,
		float RadCy,
		float HeiCy,
		DirectX::XMFLOAT3& outPCy
	);
	static bool Collision::RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const DirectX::XMFLOAT4X4& worldTransform,
		const Model* model,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);

};