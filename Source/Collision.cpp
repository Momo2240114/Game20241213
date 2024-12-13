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
	// 当たり判定の詳細処理
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

	// 地面の高さに合わせた調整（例: 0固定）
	outPB.y = 0;

	return true;
}

//球と円柱の当たり判定
bool Collision::InterctSpherVSCylinder(const DirectX::XMFLOAT3& pSp, float RadiusSp, const DirectX::XMFLOAT3& pCy, float RadCy, float HeiCy, DirectX::XMFLOAT3& outPCy)
{
	// 球の中心と円柱の軸方向の位置ベクトルを計算
	DirectX::XMVECTOR PositionSp = DirectX::XMLoadFloat3(&pSp);
	DirectX::XMVECTOR PositionCy = DirectX::XMLoadFloat3(&pCy);

	// 円柱の軸方向のY成分を固定して地面の高さを基準とする
	DirectX::XMVECTOR AxisDirection = DirectX::XMVectorSet(0, 1, 0, 0); // Y軸方向

	// 球の中心から円柱の軸に対して垂直なベクトルを求める
	DirectX::XMVECTOR VecToAxis = DirectX::XMVectorSubtract(PositionSp, PositionCy);
	DirectX::XMVECTOR VecToAxis2D = DirectX::XMVectorSet(VecToAxis.m128_f32[0], 0, VecToAxis.m128_f32[2], 0);

	// 円柱の半径内に球があるかを判定
	float distanceSq;
	DirectX::XMStoreFloat(&distanceSq, DirectX::XMVector3LengthSq(VecToAxis2D));
	float range = RadCy + RadiusSp;
	if (distanceSq > (range * range)) {
		return false; // 接触していない
	}

	// 接触点の位置を計算
	float distance = sqrt(distanceSq);
	float scale = RadCy / distance;
	DirectX::XMVECTOR ContactPoint = DirectX::XMVectorAdd(PositionCy, DirectX::XMVectorScale(VecToAxis2D, scale));
	outPCy = { ContactPoint.m128_f32[0], PositionSp.m128_f32[1], ContactPoint.m128_f32[2] };

	// 円柱の高さを考慮して接触点のY座標を調整
	if (outPCy.y < pCy.y - HeiCy / 2) {
		outPCy.y = pCy.y - HeiCy / 2;
	}
	else if (outPCy.y > pCy.y + HeiCy / 2) {
		outPCy.y = pCy.y + HeiCy / 2;
	}

	return true; // 接触している
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

// 始点と終点からレイのベクトルと長さを求める
DirectX::XMVECTOR WorldRayStart = DirectX::XMLoadFloat3(&start);
DirectX::XMVECTOR WorldRayEnd = DirectX::XMLoadFloat3(&end);
DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldRayEnd, WorldRayStart);
DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

float nearestDist = DirectX::XMVectorGetX(WorldRayLength);
if (nearestDist <= 0.0f) return false;

DirectX::XMMATRIX ParentWorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);

// モデル内の全てのメッシュと交差判定を行う
const ModelResource* resource = model->GetResource();
for (const ModelResource::Mesh& mesh : resource->GetMeshes())
{
	// メッシュのワールド行列を求める
	const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
	DirectX::XMMATRIX GlobalTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixMultiply(GlobalTransform, ParentWorldTransform);

	// レイをメッシュのローカル空間に変換する
	DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
	DirectX::XMVECTOR LocalRayStart = DirectX::XMVector3Transform(WorldRayStart, InverseWorldTransform);
	DirectX::XMVECTOR LocalRayEnd = DirectX::XMVector3Transform(WorldRayEnd, InverseWorldTransform);
	DirectX::XMVECTOR LocalRayVec = DirectX::XMVectorSubtract(LocalRayEnd, LocalRayStart);
	DirectX::XMVECTOR LocalRayDirection = DirectX::XMVector3Normalize(LocalRayVec);
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(LocalRayVec));

	// メッシュ内の全ての三角形と交差判定を行う
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		// 三角形の頂点座標を取得
		const ModelResource::Vertex& a = mesh.vertices.at(mesh.indices.at(i + 0));
		const ModelResource::Vertex& b = mesh.vertices.at(mesh.indices.at(i + 1));
		const ModelResource::Vertex& c = mesh.vertices.at(mesh.indices.at(i + 2));

		DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
		DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
		DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

		// 三角形の交差判定
		if (DirectX::TriangleTests::Intersects(LocalRayStart, LocalRayDirection, A, B, C, distance))
		{
			// メッシュのローカル空間での交点を求める
			DirectX::XMVECTOR LocalHitVec = DirectX::XMVectorScale(LocalRayDirection, distance);
			DirectX::XMVECTOR LocalHitPosition = DirectX::XMVectorAdd(LocalRayStart, LocalHitVec);

			// メッシュのローカル空間での交点をワールド空間に変換する
			DirectX::XMVECTOR WorldHitPosition = DirectX::XMVector3Transform(LocalHitPosition, WorldTransform);

			// ワールド空間でのレイの始点から交点までの距離を求める
			DirectX::XMVECTOR WorldHitVec = DirectX::XMVectorSubtract(WorldHitPosition, WorldRayStart);
			DirectX::XMVECTOR WorldHitDist = DirectX::XMVector3Length(WorldHitVec);
			float worldHitDist = DirectX::XMVectorGetX(WorldHitDist);

			// 交点までの距離が今までに計算した交点より近い場合は交差したと判定する
			if (worldHitDist <= nearestDist)
			{
				// メッシュのローカル空間での三角形の法線ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR LocalHitNormal = DirectX::XMVector3Cross(AB, BC);

				// メッシュのローカル空間での法線ベクトルをワールド空間に変換する
				DirectX::XMVECTOR WorldHitNormal = DirectX::XMVector3TransformNormal(LocalHitNormal, WorldTransform);

				// 三角形の裏表判定（内積の結果がマイナスならば表向き）
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(LocalRayDirection, LocalHitNormal);
				float dot = DirectX::XMVectorGetX(Dot);
				if (dot < 0)
				{
					// 交差したのでヒット情報を格納
					DirectX::XMStoreFloat3(&hitNormal, DirectX::XMVector3Normalize(WorldHitNormal));
					DirectX::XMStoreFloat3(&hitPosition, WorldHitPosition);

					// 交点までの距離を更新する
					nearestDist = worldHitDist;
					hit = true;
				}
			}
		}
	}
}
return hit;
}

