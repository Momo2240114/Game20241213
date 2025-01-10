#include "System/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include <imgui.h>
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "Player.h"
#include "PutBlock.h"
#include "PlayerManager.h"

// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	/*stage = new Stage();*/
	Stage::Instance().init();
	Stage::Instance().UpdateBlockTransform();
	Stage::Instance().UpdateTransform();
	PutBlock::Instance().Initialize();

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
	PlayerManager::Instance().Clear();

	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�v���C��cameraController�[�I����
	//if (player != nullptr)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	/*Player::Instance().Finalize();*/


	////�X�e�[�W�I����
	//if (stage != nullptr)
	//{
	//	delete stage;
	//	stage = nullptr;
	//}
}

// �X�V����
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
		EnemyManager::Instance().Render(rc, modelRenderer);

	}

	// 3D�f�o�b�O�`��
	{
		PlayerManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
		EnemyManager::Instance().RenderDebugPrimitive(rc, shapeRenderer);
	}

	// 2D�X�v���C�g�`��
	{

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


