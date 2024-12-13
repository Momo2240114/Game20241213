#pragma once

#include <DirectXMath.h>

// カメラコントローラー
class CameraController
{
public:
	// 更新処理
	void Update(float elapsedTime);

	// ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3		target = { 0, 0, 0 };
	DirectX::XMFLOAT3		angle = { 0, 0, 0 };
	float					rollSpeed = DirectX::XMConvertToRadians(90);
	float					range = 13.0f;

	float maxAngleX = DirectX::XMConvertToRadians(75);
	float minAngleX = DirectX::XMConvertToRadians(-15);
	float wheelDelta;
	void UpdateVer1(float elapsedTime);
	void UpdateVer2(float elapsedTime);
	void UpdateVer3(float elapsedTime);
	void UpdateVer4(float elapsedTime);
};
