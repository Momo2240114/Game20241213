#include "Projectile.h"
#include "ProjectileManager.h"


Projectile::Projectile(ProjectileManager* mana) :manager(mana) 
{
	manager->Register(this);

}

void Projectile::Destroy()
{
	manager->Remove(this);
}

void Projectile::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	renderer->RenderSphere(rc,position,radius ,DirectX::XMFLOAT4(0, 0, 1, 1));
}

void Projectile::UpdateTransform()
{

	DirectX::XMVECTOR Front, Up, Right;

	//前ベクトルを算出
	Front = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
	//仮の上ベクトル
	Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//右ベクトルを算出
	Right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Up, Front));

	//上ベクトルを算出
	Up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(Front, Right));

	//結果
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	 // 行列作成
	transform._11 = right.x * scale.x;
	transform._12 = right.y * scale.x;
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;

	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;

	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;
	transform._34 = 0.0f;

	transform._41 = position.x; // X軸の位置
	transform._42 = position.y; // Y軸の位置
	transform._43 = position.z; // Z軸の位置
	transform._44 = 1.0f;       // 同次座標


	//発射
	this->direction = front;
}
