#pragma once

#include <DirectXMath.h>


// �J�����R���g���[���[
class CameraController
{
public:
	// �X�V����
	void Update(float elapsedTime);

	// �^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
private:
	DirectX::XMFLOAT3		target = { 0, 0, 0 };
	DirectX::XMFLOAT3		angle = { DirectX::XMConvertToRadians(45), DirectX::XMConvertToRadians(45), 0 };
	int AddAng = 45;
	float					rollSpeed = DirectX::XMConvertToRadians(90);
	float	range = 50.0f;
	float currentTime = 0.0f; // �o�ߎ���
	float rotationDuration = 1.0f; // ��]�ɂ����鎞�ԁi�b�j
	bool isRotating = false; // ��]�����ǂ���
	DirectX::XMFLOAT3 targetAngle = {0,0,0}; // �ڕW�p�x
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
