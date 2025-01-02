#include "EnemySlime.h"
#include "MathUtiles.h"
#include "Player.h"
#include "ProjectileStraight.h"

// コンストラクタ
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;
	height = 1.0f;
	radius = 0.5f;

	SetWanderState();
}

// デストラクタ
EnemySlime::~EnemySlime()
{
	delete model;
}

// 更新処理
void EnemySlime::Update(float elapsedTime)
{
	switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;	
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;	
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}
	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform();

	UpdateVelocity(elapsedTime);

	Updateinvincibletimer(elapsedTime);

	projectileMana.Update(elapsedTime);
}

// 描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
	projectileMana.Render(rc, renderer);
}

void EnemySlime::OnDead()
{
	Destroy();
}

//デバックプリミティブ
void EnemySlime::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	//基底クラスのデバッグプリミティブ描画
	Enemy::RenderDebugPrimitive(rc, renderer);

	//移動範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, TerritoryOrigin, territoryRange, 1.0f,
		DirectX::XMFLOAT4(0, 1, 0, 1));

	renderer->RenderSphere(rc,targetpos,1.0f, DirectX::XMFLOAT4(1, 1, 0, 1));

	//索敵範囲をデバッグ円柱描画
	renderer->RenderCylinder(rc, position, SearthRange, 1.0f,
		DirectX::XMFLOAT4(1, 0, 0, 1));

	
}

void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	TerritoryOrigin = origin;
	territoryRange = range;
}

void EnemySlime::SetRandomTargetposition()
{
	float theta = MathUtiles::RandomRange(-DirectX::XM_PI,DirectX::XM_PI);
	float range = MathUtiles::RandomRange(0.0f,territoryRange);

	targetpos.x = TerritoryOrigin.x + sinf(theta) * range;
	targetpos.y = TerritoryOrigin.y;
	targetpos.z = TerritoryOrigin.z + cosf(theta) * range;


}

void EnemySlime::MoveToTarget(float elapsedTime, float moveSpeedRate, float turnSpeedRate)
{
	//ターゲット方向の信仰ベクトルを算出
	float vx = targetpos.x - position.x;
	float vz = targetpos.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	//移動処理
	Move(elapsedTime, vx, vz, moveSpeed * moveSpeedRate);
	Turn(elapsedTime, vx, vz, turnSpeed * turnSpeedRate);
}

void EnemySlime::SetWanderState()
{
	state = State::Wander;

	//目的地設定
	SetRandomTargetposition();

}

void EnemySlime::UpdateWanderState(float elapsedTime)
{
	//プレイヤーを索敵
	if (SearthPlayer())
	{
		//範囲内にプレイヤーがいればAttackステートに
		SetAttackState();
	}

	//目的地までのXZ平面での判定
	float vx = targetpos.x - position.x;
	float vz = targetpos.z - position.z;
	float distsq =vx * vx + vz * vz;
	if (distsq <= radius * radius)
	{
		//次の目標
		/*SetRandomTargetposition();*/
		//待機に遷移
		SetIdleState();
	}
	MoveToTarget(elapsedTime, 1.0f, 1.0f);


}

void EnemySlime::SetIdleState()
{
	state = State::Idle;

	statetimer = MathUtiles::RandomRange(3.0, 5.0f);

}

void EnemySlime::UpdateIdleState(float elapsedTime)
{
	//プレイヤーを索敵
	if (SearthPlayer())
	{
		//範囲内にプレイヤーがいればAttackステートに
		SetAttackState();
	}

	statetimer -= elapsedTime;
	if (statetimer < 0.0f)
	{
		SetWanderState();
	}
}

bool EnemySlime::SearthPlayer()
{
	//プレイヤーとの高低差を考慮して３Dでの距離判定
	const DirectX::XMFLOAT3& playerPos = { 0,0,0 }; //Player::Instance().GetPosition();
	float vx = playerPos.x - position.x;
	float vy = playerPos.y - position.y;
	float vz = playerPos.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < SearthRange )
	{
		float  distXZ = sqrtf(vx * vx + vz + vz);
		//単位化
		vx /= distXZ;
		vz /= distXZ;
		//前方ベクトル
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		//内積差で前後判定
		float dot = (frontX * vx) + (frontZ * vz);

		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

void EnemySlime::SetAttackState()
{
	state = State::Attack;

	statetimer = 0.0f;
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
	////目的地をプレイヤーに
	//targetpos = Player::Instance().GetPosition();
	////目的地へ移動
	//MoveToTarget(elapsedTime, 0.2f, 1.0f);

	////タイマー処理
	//statetimer -= elapsedTime;
	//if (statetimer < 0.0f)
	//{
	//	DirectX::XMFLOAT3 dir;
	//	dir.x = sinf(angle.y);
	//	dir.y = 0;
	//	dir.z = cosf(angle.y);

	//	//発射位置
	//	DirectX::XMFLOAT3 pos = { position.x,position.y,position.z };
	//	pos.y += height * 0.5f;


	//	ProjectileStraight* projectile = new ProjectileStraight(&projectileMana);
	//	projectile->Launch(dir, pos);

	//	statetimer = 2.0f;
	//}

	//if (!SearthPlayer())
	//{
	//	SetIdleState();
	//}
}


