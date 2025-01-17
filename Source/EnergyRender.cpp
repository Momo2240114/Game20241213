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
    // メモリを解放する
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
        Energy = -1;  // デフォルト値を設定
    }
}

void EnergyRender::AngleSet(DirectX::XMFLOAT3 cameraFront)
{
    // UIの位置とカメラの位置の差分ベクトルを計算
    float deltaX = cameraFront.x - position.x;
    float deltaZ = cameraFront.z - position.z;

    // 差分ベクトルの長さを計算（XZ平面での距離）
    float distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);

    if (distance > 0.0001f) {  // 距離がゼロに近い場合を防止
         //ベクトルを正規化（単位ベクトル化）
        deltaX /= distance;
        deltaZ /= distance;
    }
    else {
         //デフォルトの方向を設定
        deltaX = 1.0f;
        deltaZ = 0.0f;
    }

    // atan2 を使って、エネルギーUIがカメラに向く回転角度を計算（ラジアン）
    float targetAngle = atan2f(deltaZ, deltaX);

    // ラジアンをデグリーに変換
   float angleDegrees = DirectX::XMConvertToDegrees(targetAngle);

    // 回転角度をY軸に設定
    this->angle.y = angleDegrees;

    // スケール、回転、位置を反映したワールド変換行列を計算
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);  // スケール
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);  // 回転
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y + 2.0f, position.z);  // 位置（高さを2.0f加算）

    // 最終的な変換行列を計算
    DirectX::XMMATRIX UIPos = S * R * T;

    // 変換行列を保存
    DirectX::XMStoreFloat4x4(&Transform, UIPos);
}


void EnergyRender::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    if(Energy > -1 && Energy < 4)
    {
       renderer->Render(rc, Transform, EnergyUIMedel[Energy], ShaderId::Lambert);
    }
}
