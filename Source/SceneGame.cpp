#include "System/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include <imgui.h>
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include "PutBlock.h"
#include "PlayerManager.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	/*stage = new Stage();*/
	Stage::Instance().init();
	Stage::Instance().UpdateBlockTransform();
	Stage::Instance().UpdateTransform();
	PutBlock::Instance().Initialize();

	//プレイヤー初期化

	//いったいめ
	Player *One = new Player;

	One->Initialize();
	One->SetStartPos(Stage::Instance().GetStatePos());
	PlayerManager::Instance().Register(One);
	
	//カメラコントローラー初期化
	cameraController = new CameraController();

	// カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 15, -15),	//視点
		DirectX::XMFLOAT3(0, 0, 0),		//注視点
		DirectX::XMFLOAT3(0, 1, 0)		//上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//アスペクト比
		0.1f,	//クリップ距離（近）
		1000.0f	//クリップ距離（遠）
	);

	target = { 20,-3,20 };
	target.y = 0.5f;

	// エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();

	this->SetReady();
}

// 終了化
void SceneGame::Finalize()
{
	//エネミーの終了化
	EnemyManager::Instance().Clear();
	PlayerManager::Instance().Clear();

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//プレイヤcameraControllerー終了化
	//if (player != nullptr)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	/*Player::Instance().Finalize();*/


	////ステージ終了化
	//if (stage != nullptr)
	//{
	//	delete stage;
	//	stage = nullptr;
	//}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{


	if (PutBlock::Instance().PutOff())
	{
		cameraController->moveTarget(elapsedTime);
	}
	cameraController->Update(elapsedTime);
	PutBlock::Instance().Update(elapsedTime);
	Stage::Instance().Update(elapsedTime);
	PlayerManager::Instance().Update(elapsedTime);
	PopPlayer(elapsedTime);

}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		//ステージ描画
		Stage::Instance().Render(rc, modelRenderer);
		Stage::Instance().BlockRender(rc, modelRenderer);
		PutBlock::Instance().render(rc, modelRenderer);

		//プレイヤー描画

		PlayerManager::Instance().Render(rc, modelRenderer);

		// エネミー描画
		EnemyManager::Instance().Render(rc, modelRenderer);

	}

	// 3Dデバッグ描画
	{
		PlayerManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2Dスプライト描画
	{

	}
}



// GUI描画
void SceneGame::DrawGUI()
{
	PlayerManager::Instance().DrawDebugGUI();
}

void SceneGame::PopPlayer(float elapsedTime)
{
	if (PlayerManager::Instance().PopCool(elapsedTime))
	{
		Player* PlayerPuls = new Player;

		PlayerPuls->Initialize();
		PlayerPuls->SetStartPos(Stage::Instance().GetStatePos());
		PlayerManager::Instance().Register(PlayerPuls);
	}
}


