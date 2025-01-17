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
// ������
void SceneGame::Initialize()
{
	StageSaveManager::Instance().Load();
	Timer = 0;
	L = Stage::Instance().GetLevel();
	//�X�e�[�W������
	/*stage = new Stage();*/
	Stage::Instance().init();
	Stage::Instance().UpdateBlockTransform();
	Stage::Instance().UpdateTransform();
	PutBlock::Instance().Initialize();
	PlayerManager::Instance().Clear();
	PoseRender::Instance().Init();
	AudioManager::Instance().PlayBGM(BGM_Number::Game);
	AudioManager::Instance().SetVolumeBGM(BGM_Number::Game, 0.5f);
	//�v���C���[������

	//����������
	Player *One = new Player;

	One->Initialize();
	One->SetStartPos(Stage::Instance().GetStatePos());
	PlayerManager::Instance().Register(One);

	//�J�����R���g���[���[������
	cameraController = new CameraController();

	// �J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 15, -15),	//���_
		DirectX::XMFLOAT3(0, 0, 0),		//�����_
		DirectX::XMFLOAT3(0, 1, 0)		//�����
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),//����p
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),//�A�X�y�N�g��
		0.1f,	//�N���b�v�����i�߁j
		1000.0f	//�N���b�v�����i���j
	);

	target = { 20,-3,20 };
	target.y = 0.5f;

	// �G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();

	this->SetReady();
}

// �I����
void SceneGame::Finalize()
{
	//�G�l�~�[�̏I����
	EnemyManager::Instance().Clear();
	//player�̏I����
	PlayerManager::Instance().Clear();
	PutBlock::Instance().Finalize();
	Stage::Instance().Finalize();
	PoseRender::Instance().Delete();
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	if (KeyPressedOut(32))
	{
		Pose = !Pose; // Pose �̏�Ԃ𔽓]������
	}

	if (KeyPressedIn(8))//BackSpace�ŃN�C�b�N���Z�b�g
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
				PlayerManager::Instance().Clear(); // �ǉ�: ��ԃ��Z�b�g
			}
			else
			{
				SceneManager::Instance().ChangeScene(new SceneTitle);
				PlayerManager::Instance().Clear(); // �ǉ�: ��ԃ��Z�b�g
			}
		}
	}
	
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// �`�揀��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ���C�g�����i�������j
	rc.renderState = graphics.GetRenderState();

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3D���f���`��
	{
		//�X�e�[�W�`��
		Stage::Instance().Render(rc, modelRenderer);
		Stage::Instance().BlockRender(rc, modelRenderer);
		PutBlock::Instance().render(rc, modelRenderer);

		//�v���C���[�`��
		PlayerManager::Instance().Render(rc, modelRenderer);
		// �G�l�~�[�`��
		//EnemyManager::Instance().Render(rc, modelRenderer);

	}

	// 3D�f�o�b�O�`��
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
	// 2D�X�v���C�g�`��
	{
		if (Pose)
		{
			PoseRender::Instance().Render(SpriteRc);
		}
	}
	
}
// GUI�`��
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


