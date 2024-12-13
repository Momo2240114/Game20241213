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

    // ���x�Ɋ�Â��Ĉړ�
    float moveS = this->moveSpeed * elapsedTime;
    position.x += direction.x * moveS;
    position.y += direction.y * moveS;
    position.z += direction.z * moveS;

    // �^�[�����x
    float turnS = this->turnSpeed * elapsedTime;

    // �^�[�Q�b�g�܂ł̃x�N�g��
    DirectX::XMVECTOR NowPos = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(targetPos, NowPos);

    // �������v�Z
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float distance;
    DirectX::XMStoreFloat(&distance, Length);
    if (distance > 0.0001f)
    {
        // �P�ʃx�N�g�����v�Z
        Vec = DirectX::XMVector3Normalize(Vec);

        // �O���x�N�g���ƃ^�[�Q�b�g�����x�N�g���̓��ς��v�Z
        DirectX::XMVECTOR Dire = DirectX::XMLoadFloat3(&direction);
        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dire, Vec);
        float dot;
        DirectX::XMStoreFloat(&dot, Dot);
        float rot = 1.0f - dot;	//�␳�l
        //rot = 1.0f - dot;	//ImGui�ŕ\�����邽�߂Ƀ����o�[�ϐ��Ƃ���
        if (rot > turnS) rot = turnS;	//��]���x�����Arot���傫���ꍇ�́A��]���x���g��
        // ��]���K�v�ȏꍇ�̂ݏ���
        if (fabsf(rot) > 0.01f)
        {
            // ��]�����v�Z
            DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Dire, Vec);
            Axis = DirectX::XMVector3Normalize(Axis);

            // ��]�p�x���v�Z������
            /*float angle = acosf(dot);
            float rotationAmount = min(angle, turnS);*/

            // ��]�s����쐬
            DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

            // ��]�s���K�p
            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
            Transform = DirectX::XMMatrixMultiply(Rotation, Transform);
            DirectX::XMStoreFloat4x4(&transform, Transform);

            // ��]��̑O���x�N�g�����X�V
            DirectX::XMVECTOR NewDirection = DirectX::XMVector3TransformNormal(Dire, Rotation);
            NewDirection = DirectX::XMVector3Normalize(NewDirection);
            DirectX::XMStoreFloat3(&direction, NewDirection);
        }


    }
    // �ϊ��s����X�V
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
