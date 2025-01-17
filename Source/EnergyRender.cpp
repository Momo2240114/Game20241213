#include "EnergyRender.h"

void EnergyRender::Init()
{
    EnergyUIMedel[0] = new Model("Data/Model/Energy/EnergyEmpty");
    EnergyUIMedel[1] = new Model("Data/Model/Energy/Energy1");
    EnergyUIMedel[2] = new Model("Data/Model/Energy/Energy2");
    EnergyUIMedel[3] = new Model("Data/Model/Energy/Energy3");
}

void EnergyRender::deInit()
{
    // ���������������
    for (int i = 0; i < 4; i++) {
        if (EnergyUIMedel[i]) {
            delete EnergyUIMedel[i];
            EnergyUIMedel[i] = nullptr;
        }
    }
}

void EnergyRender::SetLockBlock(DirectX::XMFLOAT3 pos, int Ene)
{
    position = pos;
    if (Ene >= 0 && Ene < 4) {
        Energy = Ene;
    }
    else {
        Energy = -1;  // �f�t�H���g�l��ݒ�
    }
}

void EnergyRender::AngleSet(DirectX::XMFLOAT3 cameraFront)
{
    // UI�̈ʒu�ƃJ�����̈ʒu�̍����x�N�g�����v�Z
    float deltaX = cameraFront.x - position.x;
    float deltaZ = cameraFront.z - position.z;

    // �����x�N�g���̒������v�Z�iXZ���ʂł̋����j
    float distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);

    if (distance > 0.0001f) {  // �������[���ɋ߂��ꍇ��h�~
         //�x�N�g���𐳋K���i�P�ʃx�N�g�����j
        deltaX /= distance;
        deltaZ /= distance;
    }
    else {
         //�f�t�H���g�̕�����ݒ�
        deltaX = 1.0f;
        deltaZ = 0.0f;
    }

    // atan2 ���g���āA�G�l���M�[UI���J�����Ɍ�����]�p�x���v�Z�i���W�A���j
    float targetAngle = atan2f(deltaZ, deltaX);

    // ���W�A�����f�O���[�ɕϊ�
   float angleDegrees = DirectX::XMConvertToDegrees(targetAngle);

    // ��]�p�x��Y���ɐݒ�
    this->angle.y = angleDegrees;

    // �X�P�[���A��]�A�ʒu�𔽉f�������[���h�ϊ��s����v�Z
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);  // �X�P�[��
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);  // ��]
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y + 2.0f, position.z);  // �ʒu�i������2.0f���Z�j

    // �ŏI�I�ȕϊ��s����v�Z
    DirectX::XMMATRIX UIPos = S * R * T;

    // �ϊ��s���ۑ�
    DirectX::XMStoreFloat4x4(&Transform, UIPos);
}


void EnergyRender::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if(Energy > -1 && Energy < 4)
    {
       renderer->Render(rc, Transform, EnergyUIMedel[Energy], ShaderId::Lambert);
    }
}
