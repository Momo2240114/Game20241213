
#include "System/Input.h"
#include "CameraController.h"
#include "Camera.h"

void CameraController::Update(float elapsedTime)
{
    //ver1
    {
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //float ax = gamePad.GetAxisRX();
    //float ay = gamePad.GetAxisRY();

    //// �J�����̉�]���x
    //float speed = rollSpeed * elapsedTime;

    //// �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
    //angle.x += ay * speed;//�X�e�B�b�N�̑O��|��
    //angle.y += ax * speed;//�X�e�B�b�N�̍��E�|��

    // X���̃J������]�𐧌�
    if (angle.x < minAngleX)
    {
    	angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
    	angle.x = maxAngleX;
    }
    //// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    //if (angle.y < -DirectX::XM_PI)
    //{
    //	angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //	angle.y -= DirectX::XM_2PI;
    //}




    //// �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
    //DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //// ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
    //DirectX::XMVECTOR Front = Transform.r[2];
    //DirectX::XMFLOAT3 front;
    //DirectX::XMStoreFloat3(&front, Front);

    //// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    //DirectX::XMFLOAT3 eye;
    //eye.x = target.x - front.x * range;
    //eye.y = target.y - front.y * range;
    //eye.z = target.z - front.z * range;

    //// �J�����Ɏ��_�𒍎��_��ݒ�i�����Feye , focus , up)
    //Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
 //   UpdateVer4(elapsedTime);
}

    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();

        // �J�����̉�]���x
        float speed = rollSpeed * elapsedTime;

        // �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
        angle.x += ay * speed; //�X�e�B�b�N�̑O��|��
        angle.y += ax * speed; //�X�e�B�b�N�̍��E�|��

        // X���̃J������]�𐧌�
        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;
        }
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;
        }

        // Y���̉�]�l��-3.14�`3.14�ɔ[�܂�悤�ɂ���
        if (angle.y < -DirectX::XM_PI)
        {
            angle.y += DirectX::XM_2PI;
        }
        if (angle.y > DirectX::XM_PI)
        {
            angle.y -= DirectX::XM_2PI;
        }

        // �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
        DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

        // ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
        DirectX::XMVECTOR Front = Transform.r[2];
        DirectX::XMFLOAT3 front;
        DirectX::XMStoreFloat3(&front, Front);

        // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
        DirectX::XMFLOAT3 eye;
        eye.x = target.x - front.x * range;
        eye.y = target.y - front.y * range;
        eye.z = target.z - front.z * range;

        // �J�����Ɏ��_�𒍎��_��ݒ�i�����Feye , focus , up)
        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

        // �}�E�X�z�C�[���̓��͂��擾
        wheelDelta = Input::Instance().GetMouse().GetWheel();
        const float zoomSpeed = 5.0f; // �z�C�[�����x

        // �������z�C�[���̓��͂Ɋ�Â��ĕύX
        range -= wheelDelta * zoomSpeed;

        // �����͈̔͐���
        const float minRange = 5.0f;  // �ŏ�����
        const float maxRange = 50.0f; // �ő勗��

        if (range < minRange)
        {
            range = minRange;
        }
        if (range > maxRange)
        {
            range = maxRange;
        }

        // UpdateVer4 ���Ăяo��
        /*UpdateVer4(elapsedTime);*/
    }

}



void CameraController::UpdateVer1(float elapsedTime)
{
    Input& input = Input::Instance();

    // �}�E�X�̈ړ��ʂ��擾
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // �}�E�X�̊��x��ݒ�
    const float mouseSensitivity = 0.005f;

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
    angle.x += deltaY * mouseSensitivity; // �㉺���]���l��
    angle.y += deltaX * mouseSensitivity;



    // ��]���X���[�Y�ɂ��邽�߂̕�Ԃ�ǉ�
    static DirectX::XMFLOAT3 smoothAngle = angle;
    const float smoothFactor = 0.1f; // ���炩���𒲐�
    smoothAngle.x += (angle.x - smoothAngle.x) * smoothFactor;
    smoothAngle.y -= (angle.y - smoothAngle.y) * smoothFactor;


    // �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �}�E�X�̍��N���b�N��������Ă���Ԃ̂݃J�����𐧌�
    if (input.GetMouse().GetButton() & Mouse::BTN_LEFT)
    {
        // �}�E�X�J�[�\������ʒ����ɖ߂�
        POINT center;
        center.x = input.GetMouse().GetScreenWidth() / 2;
        center.y = input.GetMouse().GetScreenHeight() / 2;
        SetCursorPos(center.x, center.y);

        // �}�E�X�̋��ʒu����ʒ����Ƀ��Z�b�g
        input.GetMouse().Update();
    }
    // �J�����Ɏ��_�𒍎��_��ݒ�i�����Feye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer2(float elapsedTime)
{
    Input& input = Input::Instance();

    // �}�E�X�̈ړ��ʂ��擾
    static int lastMouseX = input.GetMouse().GetPositionX();
    static int lastMouseY = input.GetMouse().GetPositionY();

    int currentMouseX = input.GetMouse().GetPositionX();
    int currentMouseY = input.GetMouse().GetPositionY();

    float deltaX = static_cast<float>(currentMouseX - lastMouseX);
    float deltaY = static_cast<float>(currentMouseY - lastMouseY);

    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    // �}�E�X�̊��x��ݒ�
    const float mouseSensitivity = 0.02f; // ���x�𒲐�

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
    angle.x += deltaY * mouseSensitivity; // �㉺���]���l��
    angle.y += deltaX * mouseSensitivity;

    //// X���̃J������]�𐧌�
    //if (angle.x < minAngleX)
    //{
    //    angle.x = minAngleX;
    //}
    //if (angle.x > maxAngleX)
    //{
    //    angle.x = maxAngleX;
    //}

    //// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    //if (angle.y < -DirectX::XM_PI)
    //{
    //    angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //    angle.y -= DirectX::XM_2PI;
    //}

    // ��]���X���[�Y�ɂ��邽�߂̕�Ԃ�ǉ�
    static DirectX::XMFLOAT3 smoothAngle = angle;
    const float smoothFactor = 0.1f; // ���炩���𒲐�
    smoothAngle.x += (angle.x - smoothAngle.x) * smoothFactor;
    smoothAngle.y += (angle.y - smoothAngle.y) * smoothFactor;

    // �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(smoothAngle.x, smoothAngle.y, smoothAngle.z);

    // ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����Ɏ��_�𒍎��_��ݒ�i�����Feye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer3(float elapsedTime)
{
    Input& input = Input::Instance();

    // �}�E�X�̈ړ��ʂ��擾
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // �}�E�X�̊��x��ݒ�
    const float mouseSensitivity = 0.005f;

    // �J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    // �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
    angle.x += deltaY * mouseSensitivity; // �㉺���]���l��
    angle.y += deltaX * mouseSensitivity;

    // X���̃J������]�𐧌�
    if (angle.x < -360)
    {
        angle.x = 0;
    }
    if (angle.x > 360)
    {
        angle.x = 0;
    }

    // Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    //if (angle.y < -360)
    //{
    //    angle.y = 0;
    //}
    //if (angle.y > 360)
    //{
    //    angle.y = 0;
    //}

    // �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����Ɏ��_�𒍎��_��ݒ�i�����Feye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer4(float elapsedTime)
{
    Input& input = Input::Instance();

    // �}�E�X�̈ړ��ʂ��擾
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // �}�E�X�̊��x��ݒ�
    const float mouseSensitivity = 0.005f;

    // �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
    angle.x += deltaY * mouseSensitivity; // �㉺���]���l��
    angle.y += deltaX * mouseSensitivity;

    // �}�E�X�z�C�[���̓��͂��擾
    float wheelDelta = input.GetMouse().GetWheel();
    const float zoomSpeed = 5.0f; // �z�C�[�����x
    range -= wheelDelta * zoomSpeed;

    // �����͈̔͐���
    const float minRange = 5.0f;  // �ŏ�����
    const float maxRange = 50.0f; // �ő勗��

    // �J������]�l����]�s��ɕϊ��i�����̓s�b�`�A���[�A���[���j
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // ��]�s�񂩂�O�����x�N�g�������o���iZ���x�N�g���j
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // �J�����Ɏ��_�ƒ����_��ݒ�i�����Feye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}