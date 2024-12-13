#include "SceneLoading.h"
#include "SceneManager.h"
#include "System/Graphics.h"
#include "System/Input.h"

void SceneLoading::Initialize()
{

	sprite = new Sprite("Data/Sprite/LoadingIcon.png");

	// �X���b�h�J�n
	thread = new std::thread(&SceneLoading::LoadingThread, this);
}

void SceneLoading::Finalize()
{
	// �X���b�h�̏I����ҋ@���ĉ��
	if (thread && thread->joinable()) {
		thread->join(); // �X���b�h�̏I����ҋ@
		delete thread;  // ���������
		thread = nullptr;
	}

	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void SceneLoading::Update(float elapsedTime)
{
	constexpr float speed = 180;
	angle += speed * elapsedTime;

	//���̃V�[���̏�������������ΐ؂�ւ���
	if (Isready()) {
		SceneManager::Instance().ChangeScene(nextScene);
	}
}

void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderstate = graphics.GetRenderState();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderstate;
	//2D�X�v���C�g�`��
	{
		//�^�C�g���`��
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float sprWidth = 256;
		float sprHeight = 256;
		//float positionX = screenWidth - sprWidth;
		//float positionY = screenHeight - sprHeight;
		float positionX = (screenWidth - sprWidth) / 2; // �����ɔz�u
		float positionY = (screenHeight - sprHeight) / 2;
		sprite->Render(rc,
			positionX, positionY, 0, sprWidth, sprHeight,
			angle,
			1, 1, 1, 1
		);
	}

}

void SceneLoading::DrawGUI()
{
}

void SceneLoading::LoadingThread(SceneLoading* Scene)
{
	// COM�֘A�̏�����
	CoInitialize(nullptr);

	// ���̃V�[���̏�����
	Scene->nextScene->Initialize();

	// COM�֘A�̉��
	CoUninitialize();

	// ���̃V�[���̏���������ݒ�
	Scene->SetReady();
}
