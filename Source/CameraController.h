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
	DirectX::XMFLOAT3		angle = { DirectX::XMConvertToRadians(45), DirectX::XMConvertToRadians(45), 0 };
	int AddAng = 45;
	float					rollSpeed = DirectX::XMConvertToRadians(90);
	float	range = 50.0f;
	float currentTime = 0.0f; // 経過時間
	float rotationDuration = 1.0f; // 回転にかける時間（秒）
	bool isRotating = false; // 回転中かどうか
	DirectX::XMFLOAT3 targetAngle = {0,0,0}; // 目標角度
	float maxAngleX = DirectX::XMConvertToRadians(45);
	float minAngleX = DirectX::XMConvertToRadians(45);
	float rangeChangeSpeed = 25;
	float minRange = 30;
	float maxRange = 70;
	float wheelDelta;
	void UpdateVer1(float elapsedTime);
	void UpdateVer2(float elapsedTime);
	void UpdateVer3(float elapsedTime);
	void UpdateVer4(float elapsedTime);
};
