#include <DirectXCollision.h>
#include "Collision.h"

bool Collision::IntersectVsShaere(
	const DirectX::XMFLOAT3& pA, float RadiusA,
	const DirectX::XMFLOAT3& pB, float RadiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&pA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&pB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	float range = RadiusA + RadiusB;
	if (range * range < lengthSq)
	{
		return false;
	}
	
	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPositionB, Vec);

	return true;
}

bool Collision::IntersectVsSCylinder(const DirectX::XMFLOAT3& pA, float RadA, float HeiA, const DirectX::XMFLOAT3& pB, float RadB, float HeiB, DirectX::XMFLOAT3& outPB)
{
	// �����蔻��̏ڍ׏���
	if (pA.y <= (pB.y - HeiB) || (pA.y - HeiA) >= pB.y)
	{
		return false;
	}

	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&pA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&pB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	float range = RadA + RadB;
	if (range * range < lengthSq)
	{
		return false;
	}

	Vec = DirectX::XMVector3Normalize(Vec);
	Vec = DirectX::XMVectorScale(Vec, range);
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	DirectX::XMStoreFloat3(&outPB, Vec);

	// �n�ʂ̍����ɍ��킹�������i��: 0�Œ�j
	outPB.y = 0;

	return true;
}

//���Ɖ~���̓����蔻��
bool Collision::InterctSpherVSCylinder(const DirectX::XMFLOAT3& pSp, float RadiusSp, const DirectX::XMFLOAT3& pCy, float RadCy, float HeiCy, DirectX::XMFLOAT3& outPCy)
{
	// ���̒��S�Ɖ~���̎������̈ʒu�x�N�g�����v�Z
	DirectX::XMVECTOR PositionSp = DirectX::XMLoadFloat3(&pSp);
	DirectX::XMVECTOR PositionCy = DirectX::XMLoadFloat3(&pCy);

	// �~���̎�������Y�������Œ肵�Ēn�ʂ̍�������Ƃ���
	DirectX::XMVECTOR AxisDirection = DirectX::XMVectorSet(0, 1, 0, 0); // Y������

	// ���̒��S����~���̎��ɑ΂��Đ����ȃx�N�g�������߂�
	DirectX::XMVECTOR VecToAxis = DirectX::XMVectorSubtract(PositionSp, PositionCy);
	DirectX::XMVECTOR VecToAxis2D = DirectX::XMVectorSet(VecToAxis.m128_f32[0], 0, VecToAxis.m128_f32[2], 0);

	// �~���̔��a���ɋ������邩�𔻒�
	float distanceSq;
	DirectX::XMStoreFloat(&distanceSq, DirectX::XMVector3LengthSq(VecToAxis2D));
	float range = RadCy + RadiusSp;
	if (distanceSq > (range * range)) {
		return false; // �ڐG���Ă��Ȃ�
	}

	// �ڐG�_�̈ʒu���v�Z
	float distance = sqrt(distanceSq);
	float scale = RadCy / distance;
	DirectX::XMVECTOR ContactPoint = DirectX::XMVectorAdd(PositionCy, DirectX::XMVectorScale(VecToAxis2D, scale));
	outPCy = { ContactPoint.m128_f32[0], PositionSp.m128_f32[1], ContactPoint.m128_f32[2] };

	// �~���̍������l�����ĐڐG�_��Y���W�𒲐�
	if (outPCy.y < pCy.y - HeiCy / 2) {
		outPCy.y = pCy.y - HeiCy / 2;
	}
	else if (outPCy.y > pCy.y + HeiCy / 2) {
		outPCy.y = pCy.y + HeiCy / 2;
	}

	return true; // �ڐG���Ă���
}

bool Collision::RayCast(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const DirectX::XMFLOAT4X4& worldTransform,
	const Model* model,
	DirectX::XMFLOAT3& hitPosition,
	DirectX::XMFLOAT3& hitNormal)
{
	bool hit = false;

// �n�_�ƏI�_���烌�C�̃x�N�g���ƒ��������߂�
DirectX::XMVECTOR WorldRayStart = DirectX::XMLoadFloat3(&start);
DirectX::XMVECTOR WorldRayEnd = DirectX::XMLoadFloat3(&end);
DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldRayEnd, WorldRayStart);
DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

float nearestDist = DirectX::XMVectorGetX(WorldRayLength);
if (nearestDist <= 0.0f) return false;

DirectX::XMMATRIX ParentWorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);

// ���f�����̑S�Ẵ��b�V���ƌ���������s��
const ModelResource* resource = model->GetResource();
for (const ModelResource::Mesh& mesh : resource->GetMeshes())
{
	// ���b�V���̃��[���h�s������߂�
	const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
	DirectX::XMMATRIX GlobalTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixMultiply(GlobalTransform, ParentWorldTransform);

	// ���C�����b�V���̃��[�J����Ԃɕϊ�����
	DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
	DirectX::XMVECTOR LocalRayStart = DirectX::XMVector3Transform(WorldRayStart, InverseWorldTransform);
	DirectX::XMVECTOR LocalRayEnd = DirectX::XMVector3Transform(WorldRayEnd, InverseWorldTransform);
	DirectX::XMVECTOR LocalRayVec = DirectX::XMVectorSubtract(LocalRayEnd, LocalRayStart);
	DirectX::XMVECTOR LocalRayDirection = DirectX::XMVector3Normalize(LocalRayVec);
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(LocalRayVec));

	// ���b�V�����̑S�Ă̎O�p�`�ƌ���������s��
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		// �O�p�`�̒��_���W���擾
		const ModelResource::Vertex& a = mesh.vertices.at(mesh.indices.at(i + 0));
		const ModelResource::Vertex& b = mesh.vertices.at(mesh.indices.at(i + 1));
		const ModelResource::Vertex& c = mesh.vertices.at(mesh.indices.at(i + 2));

		DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
		DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
		DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

		// �O�p�`�̌�������
		if (DirectX::TriangleTests::Intersects(LocalRayStart, LocalRayDirection, A, B, C, distance))
		{
			// ���b�V���̃��[�J����Ԃł̌�_�����߂�
			DirectX::XMVECTOR LocalHitVec = DirectX::XMVectorScale(LocalRayDirection, distance);
			DirectX::XMVECTOR LocalHitPosition = DirectX::XMVectorAdd(LocalRayStart, LocalHitVec);

			// ���b�V���̃��[�J����Ԃł̌�_�����[���h��Ԃɕϊ�����
			DirectX::XMVECTOR WorldHitPosition = DirectX::XMVector3Transform(LocalHitPosition, WorldTransform);

			// ���[���h��Ԃł̃��C�̎n�_�����_�܂ł̋��������߂�
			DirectX::XMVECTOR WorldHitVec = DirectX::XMVectorSubtract(WorldHitPosition, WorldRayStart);
			DirectX::XMVECTOR WorldHitDist = DirectX::XMVector3Length(WorldHitVec);
			float worldHitDist = DirectX::XMVectorGetX(WorldHitDist);

			// ��_�܂ł̋��������܂łɌv�Z������_���߂��ꍇ�͌��������Ɣ��肷��
			if (worldHitDist <= nearestDist)
			{
				// ���b�V���̃��[�J����Ԃł̎O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR LocalHitNormal = DirectX::XMVector3Cross(AB, BC);

				// ���b�V���̃��[�J����Ԃł̖@���x�N�g�������[���h��Ԃɕϊ�����
				DirectX::XMVECTOR WorldHitNormal = DirectX::XMVector3TransformNormal(LocalHitNormal, WorldTransform);

				// �O�p�`�̗��\����i���ς̌��ʂ��}�C�i�X�Ȃ�Ε\�����j
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(LocalRayDirection, LocalHitNormal);
				float dot = DirectX::XMVectorGetX(Dot);
				if (dot < 0)
				{
					// ���������̂Ńq�b�g�����i�[
					DirectX::XMStoreFloat3(&hitNormal, DirectX::XMVector3Normalize(WorldHitNormal));
					DirectX::XMStoreFloat3(&hitPosition, WorldHitPosition);

					// ��_�܂ł̋������X�V����
					nearestDist = worldHitDist;
					hit = true;
				}
			}
		}
	}
}
return hit;
}

