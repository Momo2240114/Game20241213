#include "ProjectileHoming.h"

ProjectileHoming::ProjectileHoming(ProjectileManager* Mana)
	:Projectile(Mana)
{
	model = new Model("Data/Model/Sword/Sword.mdl");

	scale.x = scale.y = scale.z = 3.5f;
}

ProjectileHoming::~ProjectileHoming()
{
	delete model;
}
void ProjectileHoming::Update(float elapsedTime)
{
    lifetimer -= elapsedTime;
    if (lifetimer < 0)
    {
        Destroy();
    }

    // 速度に基づいて移動
    float moveS = this->moveSpeed * elapsedTime;
    position.x += direction.x * moveS;
    position.y += direction.y * moveS;
    position.z += direction.z * moveS;

    // ターン速度
    float turnS = this->turnSpeed * elapsedTime;

    // ターゲットまでのベクトル
    DirectX::XMVECTOR NowPos = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(targetPos, NowPos);

    // 距離を計算
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float distance;
    DirectX::XMStoreFloat(&distance, Length);
    if (distance > 0.0001f)
    {
        // 単位ベクトルを計算
        Vec = DirectX::XMVector3Normalize(Vec);

        // 前方ベクトルとターゲット方向ベクトルの内積を計算
        DirectX::XMVECTOR Dire = DirectX::XMLoadFloat3(&direction);
        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dire, Vec);
        float dot;
        DirectX::XMStoreFloat(&dot, Dot);
        float rot = 1.0f - dot;	//補正値
        //rot = 1.0f - dot;	//ImGuiで表示するためにメンバー変数とした
        if (rot > turnS) rot = turnS;	//回転速度よりも、rotが大きい場合は、回転速度を使う
        // 回転が必要な場合のみ処理
        if (fabsf(rot) > 0.01f)
        {
            // 回転軸を計算
            DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Dire, Vec);
            Axis = DirectX::XMVector3Normalize(Axis);

            // 回転角度を計算し制限
            /*float angle = acosf(dot);
            float rotationAmount = min(angle, turnS);*/

            // 回転行列を作成
            DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

            // 回転行列を適用
            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
            Transform = DirectX::XMMatrixMultiply(Rotation, Transform);
            DirectX::XMStoreFloat4x4(&transform, Transform);

            // 回転後の前方ベクトルを更新
            DirectX::XMVECTOR NewDirection = DirectX::XMVector3TransformNormal(Dire, Rotation);
            NewDirection = DirectX::XMVector3Normalize(NewDirection);
            DirectX::XMStoreFloat3(&direction, NewDirection);
        }


    }
    // 変換行列を更新
    UpdateTransform();

    model->UpdateTransform();
}

void ProjectileHoming::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{
	this->direction = direction;
	this->position = position;
	this->target = target;

	UpdateTransform();
}
