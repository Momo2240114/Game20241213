#include "Character.h"
#include "Stage.h"

//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y,scale.z);

    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // ３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    // 計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//速力に力を与える
	Velocity.x += impulse.x;
	Velocity.y += impulse.y;
	Velocity.z += impulse.z;
}

void Character::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	renderer->RenderCylinder(rc, position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Character::ApplyDamage(int damage, float invincibletime)
{
	if (damage == 0)return false;

	if (Health == 0)return false;

	if (invincibletimer > 0) return false;

	invincibletimer = invincibletime;

	Health -= damage;

	if (Health == 0)//脂肪判定
	{
		OnDead();
	}
	else//ダメージ判定
	{
		OnDamaged();
	}
	//体力が変わると
	return true;
}

void Character::HandleBlockCollision(int blockID, const DirectX::XMFLOAT3& hitPosition, const DirectX::XMFLOAT3& hitNormal)
{
	//switch (blockID) {
	//case 1:
	//	// ギミック1: 例) キャラクターの動きを止める
	//	Velocity = { 0.0f, 0.0f, 0.0f };
	//	break;
	//case 2:
	//	// ギミック2: 例) キャラクターを跳ね返す
	//	Velocity.y = 10.0f; // 上向きの速度を付与
	//	break;

	//case 3:
	//	// ギミック3: 例) 特定のアクションを発動する
	//	/*TriggerSpecialAction();*/
	//	break;

	//default:
	//	// 他のブロックIDや未定義の動作
	//	break;
	//}
}

void Character::UpdateVerticalVelocity(float elapsedTime)
{
	//重力処理
	Velocity.y += gravity * elapsedTime;
}

void Character::UpdateHorizonVelocity(float elapsedTime)
{
	////XZ平面の速力を減速する
	float Length = sqrtf(Velocity.x * Velocity.x + Velocity.z * Velocity.z);
	if (Length > 0.0f)
	{
		//摩擦力
		float Friction = this->friction * elapsedTime;

		//空中にいるときは摩擦力を減らす
		if (!IsGround()) // IsInAirフラグを用いる
		{
			Friction *= airControl; // 空中では摩擦力を30%に減少
		}
			


		//摩擦による後方向の減速
		if (Length > Friction)
		{
			 //摩擦を適用して速度を減少させる
			float decelerationFactor = (Length - Friction) / Length;
			Velocity.x *= decelerationFactor;
			Velocity.z *= decelerationFactor;
		}
		//横方向の速力が摩擦以下になったとき速力を無効化
		else
		{
			Velocity.x = 0.0f;
			Velocity.z = 0.0f;
		}
	}
	// 最大速度未満の場合の加速処理
	float moveVecLength = sqrtf(MoveVecX * MoveVecX + MoveVecZ * MoveVecZ);
	if (moveVecLength > 0.0f && Length <= MaxMoveSpeed)
	{
		// 加速力を適用
		float acceleration = this->acceleration * elapsedTime;
		Velocity.x += MoveVecX * acceleration;
		Velocity.z += MoveVecZ * acceleration;

		// 最大速度制限
		float newLength = sqrtf(Velocity.x * Velocity.x + Velocity.z * Velocity.z);
		if (newLength > MaxMoveSpeed)
		{
			// 正規化して速度を制限
			float normalizationFactor = MaxMoveSpeed / newLength;
			Velocity.x *= normalizationFactor;
			Velocity.z *= normalizationFactor;
		}
	}

	// 移動ベクトルをリセット
	MoveVecX = 0.0f;
	MoveVecZ = 0.0f;
}


void Character::UpdateVerticalMove(float elapsedTime)
{
	//前のやつ
	{
		////移動処理
		//position.y += Velocity.y * elapsedTime;

		//if (position.y < 0.0f)
		//{
		//	position.y = 0;
		//	Velocity.y = 0;

		//	if (!IsGround())
		//	{
		//		OnLanding();
		//	}
		//	isGround = true;
		//}
		//else
		//{
		//	isGround = false;
		//}
	}
	//頭なし
	{
	//	float my = Velocity.y * elapsedTime;

	//	if (my < 1.0f)
	//	{
	//		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
	//		DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y;

	//			if (!isGround)
	//			{
	//				OnLanding();
	//			}
	//			isGround = true;
	//			Velocity.y = 0.0f;
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//	else if (my > 0.0f)
	//	{
	//		position.y += my;
	//		isGround = false;
	//	}
	}

	int HitBlock = 0;
	float my = Velocity.y * elapsedTime;

	bool BackframeisGround = isGround;
	isGround = false;

	if (my < 1.0f) // 下方向の移動（落下）
	{
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset * 0.2f, position.z };
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		DirectX::XMFLOAT3 hitPosition;
		DirectX::XMFLOAT3 hitNormal;
		DirectX::XMFLOAT3 BlockAngle;

		if (BackframeisGround)
		{
			end.y += my * 0.3f;
		}

		if (Stage::Instance().UnifiedRayCast(start, end, hitPosition, hitNormal, HitBlock,true))
		{
			switch (HitBlock)
			{
			case 0:
			case 1:
			case 2:
				position.y = hitPosition.y;

				if (!isGround)
				{
					OnLanding();
				}
				isGround = true;
				Velocity.y = 0.0f;
				break;
			case 3:
				position.y = hitPosition.y;
				isGround = true;
				Jump(16);
				break;
			case 4:
				break;
			}
			
		}
		else
		{
			position.y += my;
			isGround = false;
		}
	}


	//{
	//	if (my < 1.0f) // 下方向の移動（落下）
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset/2, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y;

	//			if (!isGround)
	//			{
	//				OnLanding();
	//			}
	//			isGround = true;
	//			Velocity.y = 0.0f;
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//	if (my < 1.0f) // 下方向の移動（落下）
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;
	//		if (Stage::Instance().BlockRayCast(start, end, hitPosition, hitNormal, HitBlock))
	//		{
	//			switch (HitBlock)
	//			{
	//			case 1:
	//			case 2:
	//				position.y = hitPosition.y;

	//				if (!isGround)
	//				{
	//					OnLanding();
	//				}
	//				isGround = true;
	//				Velocity.y = 0.0f;
	//				break;
	//			case 3:
	//				position.y = hitPosition.y;
	//				isGround = true;
	//				Jump(16);
	//				break;
	//			}

	//		}
	//	}
	//	else if (my > 0.0f) // 上方向の移動（ジャンプ）
	//	{
	//		DirectX::XMFLOAT3 start = { position.x, position.y + height - stepOffset, position.z };
	//		DirectX::XMFLOAT3 end = { position.x, position.y + height + my, position.z };

	//		DirectX::XMFLOAT3 hitPosition;
	//		DirectX::XMFLOAT3 hitNormal;

	//		if (Stage::Instance().RayCast(start, end, hitPosition, hitNormal))
	//		{
	//			position.y = hitPosition.y - height - my; // 頭が天井に接触した場合、頭の高さ分調整

	//			isGround = false;
	//			Velocity.y = 0.0f; // 上方向の速度をゼロにする
	//		}
	//		else
	//		{
	//			position.y += my;
	//			isGround = false;
	//		}
	//	}
	//}



}

void Character::UpdateHorizonMove(float elapsedTime)
{
	//水平方向の移動処理
	//position.x += Velocity.x * elapsedTime;
	//position.z += Velocity.z * elapsedTime;
	float moveX = Velocity.x * elapsedTime;
	float moveZ = Velocity.z * elapsedTime;
	float moveLength = sqrtf(moveX * moveX + moveZ * moveZ);
	//頭あり（重い）
	{
		//float moveX = Velocity.x * elapsedTime;
		//float moveZ = Velocity.z * elapsedTime;
		//float moveLength = sqrtf(moveX * moveX + moveZ * moveZ);
		//// 足元のレイ
		//const DirectX::XMFLOAT3 sFeet = { position.x, position.y + 0.3f, position.z };
		//const DirectX::XMFLOAT3 eFeet = { position.x + moveX * 3, position.y + 0.3f, position.z + moveZ * 3 };

		//// 頭付近のレイ
		//const DirectX::XMFLOAT3 sHead = { position.x, position.y + height - 0.3f, position.z };
		//const DirectX::XMFLOAT3 eHead = { position.x + moveX * 3, position.y + height - 0.3f, position.z + moveZ * 3 };
		////法線ベクトルと交点の保存先
		//DirectX::XMFLOAT3 pFeet, nFeet;
		//DirectX::XMFLOAT3 pHead, nHead;


		//bool shouldRayCast = (position.x != Previousposition.x || position.z != Previousposition.z);
		//Previousposition = position;

		////頭と足元どちらが当たったかしらべる
		//bool hitFeet = Stage::Instance().RayCast(sFeet, eFeet, pFeet, nFeet);
		//bool hitHead = Stage::Instance().RayCast(sHead, eHead, pHead, nHead);


		//if (hitFeet || hitHead)
		//{
		//	// 足元または頭がヒットした場合、衝突点と法線を取得
		//	DirectX::XMFLOAT3 p = hitFeet ? pFeet : pHead;
		//	DirectX::XMFLOAT3 n = hitFeet ? nFeet : nHead;

		//	// 衝突点と現在位置の差分を計算
		//	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
		//	DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eFeet);
		//	DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

		//	DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
		//	DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

		//	float a = DirectX::XMVectorGetX(A) + 0.5f; // めり込みを広く取る
		//	DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
		//	DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

		//	DirectX::XMFLOAT3 q;
		//	DirectX::XMStoreFloat3(&q, Q);

		//	// 衝突点で再度レイキャストを行って修正された位置を計算
		//	if (Stage::Instance().RayCast(sFeet, q, pFeet, nFeet))
		//	{
		//		DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
		//		DirectX::XMFLOAT3 correctedPositionFloat3;
		//		DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

		//		position.x = correctedPositionFloat3.x;
		//		position.z = correctedPositionFloat3.z;
		//	}
		//	else
		//	{
		//		position.x = q.x;
		//		position.z = q.z;
		//	}
		//}
		//else
		//{
		//	position.x += moveX;
		//	position.z += moveZ;
		//}

		//float vx = moveX / moveLength;
		//float vz = moveZ / moveLength;
		//angle.y = atan2f(vx, vz);
	}
	// 足元のレイ
	{
		const DirectX::XMFLOAT3 sFeet = { position.x, position.y + stepOffset, position.z };
		const DirectX::XMFLOAT3 eFeet = { position.x + moveX * 2, position.y + stepOffset, position.z + moveZ * 2 };

		// 法線ベクトルと交点の保存先
		DirectX::XMFLOAT3 pFeet, nFeet;
		int HitBlock = 0;
		//bool shouldRayCast = (position.x != Previousposition.x || position.z != Previousposition.z);

		// 足元の当たり判定だけ行う
		bool hitFeet = false;
		bool hitBlockFeet = false;
		hitFeet = Stage::Instance().UnifiedRayCast(sFeet, eFeet, pFeet, nFeet, HitBlock,true);

		if (hitFeet)
		{
			Velocity.x = 0;
			Velocity.z = 0;
			 //足元がヒットした場合、衝突点と法線を取得
			DirectX::XMFLOAT3 p = pFeet;
			DirectX::XMFLOAT3 n = nFeet;

			 //衝突点と現在位置の差分を計算
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&p);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&eFeet);
			DirectX::XMVECTOR PE = DirectX::XMVectorSubtract(E, P);

			DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&n);
			DirectX::XMVECTOR A = DirectX::XMVector3Dot(PE, N);

			float a = DirectX::XMVectorGetX(A) - 0.09f; // めり込みを広く取る
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(PE, DirectX::XMVectorScale(N, a));
			DirectX::XMVECTOR Q = DirectX::XMVectorAdd(P, R);

			DirectX::XMFLOAT3 q;
			DirectX::XMStoreFloat3(&q, Q);

			 //衝突点で再度レイキャストを行って修正された位置を計算
			if (Stage::Instance().UnifiedRayCast(sFeet, q, pFeet, nFeet, HitBlock,true))
			{
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
				DirectX::XMFLOAT3 correctedPositionFloat3;
				DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

				position.x = correctedPositionFloat3.x;
				position.z = correctedPositionFloat3.z;
			}
			else
			{
				position.x = q.x;
				position.z = q.z;
			}
			if (Stage::Instance().RayCast(sFeet, q, pFeet, nFeet))
			{
				DirectX::XMVECTOR correctedPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pFeet), DirectX::XMVectorScale(N, 0.05f));
				DirectX::XMFLOAT3 correctedPositionFloat3;
				DirectX::XMStoreFloat3(&correctedPositionFloat3, correctedPosition);

				position.x = correctedPositionFloat3.x;
				position.z = correctedPositionFloat3.z;
			}
			else
			{
				position.x = q.x;
				position.z = q.z;
			}
		}
		else
		{
			position.x += moveX;
			position.z += moveZ;
		}

	}
}



void Character::Jump(float speed)
{
	Velocity.y = speed;
}

void Character::UpdateVelocity(float elapsedTime)
{
	//垂直の速力更新
	UpdateVerticalVelocity(elapsedTime);
	//水平の速力更新
	 UpdateHorizonVelocity(elapsedTime);
	//垂直の移動更新
	UpdateVerticalMove(elapsedTime);
	//垂直の移動更新
	UpdateHorizonMove(elapsedTime);

	{

		//Velocity.y += gravity * elapsedTime;

		//position.y += Velocity.y * elapsedTime;

		/*if (position.y < 0.0f)
		{
			position.y = 0;
			Velocity.y = 0;

			if (!IsGround())
			{
				OnLanding();
			}
			isGround = true;
		}
		else
		{
			isGround = false;
		}*/
	}

	UpdateTransform();
}

void Character::Updateinvincibletimer(float elapsedTime)
{
	if (invincibletimer > 0.0f)
	{
		invincibletimer -= elapsedTime;
	}
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;
	
	// 進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//--- ガタつきに対応させる ---
	
		// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);	//内積：フロントが基準

	// 内積値は-1.0～1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;	//補正値
	//rot = 1.0f - dot;	//ImGuiで表示するためにメンバー変数とした

	float reotationSpeed = speed * (1.0f - fabsf(dot));
	if (rot > speed) rot = speed;	//回転速度よりも、rotが大きい場合は、回転速度を使う

	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	//左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		//angle.y -= speed;
		angle.y -= rot;
	}
	else
	{
		//angle.y += speed;
		angle.y += rot;
	}
	
}

void Character::Move(float elapsedTime, float vx, float vz, float speed)
{
	//speed *= elapsedTime;
	//position.x += vx * speed;
	//position.z += vz * speed;

	MoveVecX = vx; // vx方向の移動入力
	MoveVecZ = vz; // vz方向の移動入力

	MaxMoveSpeed = speed; // 最大移動速度を設定
}