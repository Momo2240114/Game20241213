#include "Player.h"
#include "System/Input.h"
#include <imgui.h>
#include "Camera.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include <DirectXMath.h>
#include "ProjectileHoming.h"
#include "System/Graphics.h"
#include "Stage.h"

int Player::GoalNum = 0;

void Player::OnLanding()
{
	JumpCount = 0;
}

// コンストラクタ
//Player::Player()
//{
//	model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
//
//	// モデルが大きいのでスケーリング
//	scale.x = scale.y = scale.z = 0.01f;
//}
//// デストラクタ
//Player::~Player()
//{
//	delete model;
//}


// コンストラクタの代わり
void Player::Initialize()
{
	model = new Model("Data/Model/Player/Player.mdl");

	// モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.2f;

	position.y = 5;

	Startpos = position;
}

// デストラクタの代わり
void Player::Finalize()
{
	delete model;
}

void Player::Update(float elapsedTime)
{
	if (!IsGoal)
	{
		// 移動入力処理
		InputMove(elapsedTime);
		//InputJump();
		UpdateVelocity(elapsedTime);
	}

	DeadTime(elapsedTime);
	projectileManager.Update(elapsedTime);
	// オブジェクト行列を更新
	UpdateTransform();
	// モデル行列更新
	model->UpdateTransform();

	CollisionPlayerVSEnemys();

	CollisionProjectileVsEnemies();

	if (Velocity.x > 0)
	{
		if (Velocity.x >= Velocity.z)
		{
			Velocity.z = 0;
			Velocity.x = moveSpeed * accel;
		}
	}
	if (Velocity.x < 0)
	{
		if (Velocity.x < Velocity.z)
		{
			Velocity.z = 0;
			Velocity.x = -moveSpeed * accel;
		}
	}
	if(Velocity.z > 0)
	{
		if (Velocity.x <= Velocity.z)
		{
			Velocity.x = 0;
			Velocity.z = moveSpeed * accel;
		}
	}
	if(Velocity.z < 0)
	{
		if (Velocity.x > Velocity.z)
		{
			Velocity.x = 0;
			Velocity.z = -moveSpeed * accel;
		}
	}

}

void Player::Render(const RenderContext& rc, ModelRenderer* renderer)
{

	renderer->Render(rc, transform, model, ShaderId::Lambert);

	projectileManager.Render(rc, renderer);
}

// デバッグ用GUI描画
void Player::DrawDebugGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::InputFloat3("Position", &position.x);

			// 回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);

			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);

			// スケール
			ImGui::InputFloat3("Scale", &scale.x);

			// 回転値
			//ImGui::InputFloat("rot", &rot);

		}
	}

	ImGui::End();
}

void Player::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
	projectileManager.RenderDebugPrimitive(rc, renderer);
}

void Player::DeadTime(float elapsedTime)
{
	if (IsMove && Velocity.x == 0 && Velocity.z == 0)
	{
		DeadTimer += elapsedTime;
		if (DeadTimer > 3)
		{
			IsLive = true;
		}
	}
}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{

	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルなるようにする

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;

	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
	vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;

	//   // 入力情報を取得
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisLX();
	//float ay = gamePad.GetAxisLY();

	//// カメラ方向を取得
	//Camera& camera = Camera::Instance();
	//DirectX::XMFLOAT3 cameraRight = camera.GetRight();
	//DirectX::XMFLOAT3 cameraFront = camera.GetFront();

	//// カメラ方向を単位ベクトルに正規化（XZ平面）
	//DirectX::XMFLOAT3 moveVec = {
	//	cameraFront.x * ay + cameraRight.x * ax,
	//	0.0f,  // Y軸は無視
	//	cameraFront.z * ay + cameraRight.z * ax
	//};

	//// 移動ベクトルを正規化
	//float length = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
	//if (length > 0.0f) {
	//	moveVec.x /= length;
	//	moveVec.z /= length;
	//}

	//return moveVec;
}


void Player::CollisionProjectileVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	int projectileCount = projectileManager.GetprojectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* Pro = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* Ene = enemyManager.GetEnemy(j);
			DirectX::XMFLOAT3 OutP;
			if (Collision::InterctSpherVSCylinder(
				Pro->GetPosition(),
				Pro->GetRadius(),
				Ene->GetPosition(),
				Ene->GetRadius(),
				Ene->GetHeight(),
				OutP))
			{
				if (Ene->ApplyDamage(1, 30.0f))
				{
					//吹き飛ばす
					{
						//// 弾丸の方向から敵を吹き飛ばす方向を計算
						DirectX::XMFLOAT3 ProPos = Pro->GetPosition();
						DirectX::XMFLOAT3 EnePos = Ene->GetPosition();
						DirectX::XMFLOAT3 Impulse = {
							EnePos.x - ProPos.x,
							EnePos.y - ProPos.y,
							EnePos.z - ProPos.z
						};

						// 正規化して一定の吹き飛ばし強度を適用
						float length = sqrtf(Impulse.x * Impulse.x + Impulse.y * Impulse.y + Impulse.z * Impulse.z);
						if (length > 0.0f)
						{
							Impulse.x /= length;
							Impulse.y /= length;
							Impulse.z /= length;
						}

						// 強度を適用 (吹き飛ばしの強さは調整可能)
						float pushStrength = 10.0f; // 吹き飛ばし力の強さ
						Impulse.x *= pushStrength;
						Impulse.y *= pushStrength;
						Impulse.z *= pushStrength;

						Ene->AddImpulse(Impulse); // 吹き飛ばしベクトルを適用

					}

					Pro->Destroy();

				}
			}

		}
	}
}

// 移動入力処理
void Player::InputMove(float elapsedTime)
{
	//移動フラグの管理
	if (Startpos.x != position.x || Startpos.z != position.z)
	{
		IsMove = true;
	}

	//移動開始OK　Enter　OR　Second3↑
	if (IsKeyPressed(VK_RETURN) || OnMovingFloorTime > 3)
	{
		if (moveState == 0)
		{
			angle.y = round(angle.y / 90.0) * 90; // 90度単位で丸める
			if (angle.y > 360) {
				angle.y -= 360;
			}
			else if (angle.y < -0) {
				angle.y += 360;
			}
		}
		moveState = 1;
	
	}

	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = { 0,0,0 };
	if (!IsMove)
	{
		moveVec = GetMoveVec();
		OnMovingFloorTime += elapsedTime;
	}
	// 移動処理
	if(moveState == 1)
	{
		if (Velocity.x != 0 || Velocity.z != 0 && !IsMove) moveVec = Velocity;
		Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
	}
	// 旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);


}

void Player::InputJump()
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		
		if (JumpCount < JumpLimit)
		{
			Jump(jumpSpeed);
		}
		JumpCount++;
	}

}

void Player::InputProjectile()
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	if(gamepad.GetButtonDown() & GamePad::BTN_X)
	{
		DirectX::XMFLOAT3 dir = {0,0,0};
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);

		//発射位置
		DirectX::XMFLOAT3 pos = {position.x,position.y,position.z };
		pos.y +=  height * 0.5f;


		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		DirectX::XMFLOAT3 dir;

		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);

		//発射位置
		DirectX::XMFLOAT3 pos = { position.x,position.y,position.z };
		pos.y += height * 0.5f;

		//ターゲット（デフォルトはプレイヤーの前方）
		DirectX::XMFLOAT3 target =
		{position.x + dir.x * 1000,
		 position.y + dir.y * 1000,
		 position.z + dir.z * 1000 };

		//一番近くの敵をターゲットに
		float dist = FLT_MAX;
		EnemyManager& enemymanager = EnemyManager::Instance();
		int enemyCount = enemymanager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)
		{
			//敵との距離判定

			 // 敵の位置を取得
			Enemy* enemy = enemymanager.GetEnemy(i);
			DirectX::XMFLOAT3 enemyPos = enemy->GetPosition();

			// 距離の二乗を計算
			DirectX::XMVECTOR PPos = DirectX::XMLoadFloat3(&pos);
			DirectX::XMVECTOR EPos = DirectX::XMLoadFloat3(&enemyPos);

			DirectX::XMVECTOR D = DirectX::XMVectorSubtract(PPos, EPos);
			float dx = enemyPos.x - pos.x;
			float dy = enemyPos.y - pos.y;
			float dz = enemyPos.z - pos.z;
			float distSq = dx * dx + dy * dy + dz * dz;

			// 最短距離の敵を更新
			if (distSq < dist)
			{
				dist = distSq;
				target = enemyPos;
			}
		}

		// ターゲットが見つからなかった場合は発射中止
		//if (dist == FLT_MAX)
		//{
		//	return; // ターゲットがない場合の処理
		//}

		//発射
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos,target);
	}
}


void Player::CollisionPlayerVSEnemys()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	int enmyCount = enemyManager.Instance().GetEnemyCount();

	for (int i = 0; i < enmyCount; i++)
	{
		Enemy* enemy = enemyManager.Instance().GetEnemy(i);
		DirectX::XMFLOAT3 outposition;
		//if (Collision::IntersectVsShaere(
		//	position, radius,
		//	enemy->GetPosition(), enemy->GetRadius(),
		//	outposition))
		//{
		// 
		//	enemy->SetPosition(outposition);
		//}
		if (Collision::IntersectVsSCylinder(
			position, radius, height,
			enemy->GetPosition(), enemy->GetRadius(),
			enemy->GetHeight(),
			outposition))
		{
			DirectX::XMFLOAT3 EP = enemy->GetPosition();
			DirectX::XMVECTOR PlayP = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR EneP = DirectX::XMLoadFloat3(&EP);

			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PlayP,EneP);
			Vec = DirectX::XMVector3Normalize(Vec);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, Vec);
			if (normal.y > 0.6f)
			{
				Jump(normal.y * (jumpSpeed));
			}
			else
			{
				enemy->SetPosition(outposition);
			}
		}
	}
}


void Player::SetStartPos(DirectX::XMFLOAT3 State)
{
	Startpos = State;
	position = Startpos;
}


