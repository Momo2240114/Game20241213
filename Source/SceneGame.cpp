#include "System/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include <imgui.h>
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include "PutBlock.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "SceneStageSelect.h"
#include "SceneTitle.h"
#include "StageSave.h"
#include "KeyPut.h"
#include "PoseRender.h"
#include "EnergyRender.h"
#include "AudioManager.h"
// 初期化
void SceneGame::Initialize()
{
	StageSaveManager::Instance().Load();
	Timer = 0;
	L = Stage::Instance().GetLevel();
	//ステージ初期化
	/*stage = new Stage();*/
	Stage::Instance().init();
	Stage::Instance().UpdateBlockTransform();
	Stage::Instance().UpdateTransform();
	PutBlock::Instance().Initialize();
	PlayerManager::Instance().Clear();
	PoseRender::Instance().Init();
	AudioManager::Instance().PlayBGM(BGM_Number::Game);
	AudioManager::Instance().SetVolumeBGM(BGM_Number::Game, 0.5f);
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
	//playerの終了化
	PlayerManager::Instance().Clear();
	PutBlock::Instance().Finalize();
	Stage::Instance().Finalize();
	PoseRender::Instance().Delete();
	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	if (KeyPressedOut(32))
	{
		Pose = !Pose; // Pose の状態を反転させる
	}

	if (KeyPressedIn(8))//BackSpaceでクイックリセット
	{
		AudioManager::Instance().StopBGM();
		SceneManager::Instance().ChangeScene(new SceneGame);
	}


	if (!Pose) {
		Timer += elapsedTime;
		if (PutBlock::Instance().PutOff())
		{
			cameraController->moveTarget(elapsedTime);
		}
		cameraController->Update(elapsedTime);
		PutBlock::Instance().Update(elapsedTime);
		Stage::Instance().Update(elapsedTime);
		PlayerManager::Instance().Update(elapsedTime);
		PopPlayer(elapsedTime);
		if (PlayerManager::Instance().EndGame() || PlayerManager::Instance().GameClear())
		{
			AudioManager::Instance().StopBGM();
			if (PlayerManager::Instance().GameClear())
			{
				int cost = Stage::Instance().GetUseCost();
				StageSaveManager::Instance().SetStageCleared(L, true);
				StageSaveManager::Instance().SetShortestTime(L, Timer);
				StageSaveManager::Instance().SetLowestCost(L, cost);
				StageSaveManager::Instance().Save();
				SceneManager::Instance().ChangeScene(new SceneTitle);
				PlayerManager::Instance().Clear(); // 追加: 状態リセット
			}
			else
			{
				SceneManager::Instance().ChangeScene(new SceneTitle);
				PlayerManager::Instance().Clear(); // 追加: 状態リセット
			}
		}
	}
	
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
		//EnemyManager::Instance().Render(rc, modelRenderer);

	}

	// 3Dデバッグ描画
	{
		PlayerManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}


	Graphics& SpriteGraphics = Graphics::Instance();
	ID3D11DeviceContext* Spritedc = SpriteGraphics.GetDeviceContext();
	RenderState* Spriterenderstate = SpriteGraphics.GetRenderState();

	RenderContext SpriteRc;
	SpriteRc.deviceContext = Spritedc;
	SpriteRc.renderState = Spriterenderstate;
	// 2Dスプライト描画
	{
		if (Pose)
		{
			PoseRender::Instance().Render(SpriteRc);
		}
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


