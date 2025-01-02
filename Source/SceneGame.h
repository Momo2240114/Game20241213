#pragma once
#include "Stage.h"
//#include "Player.h"
#include "CameraController.h"
#include "Scene.h"

// �Q�[���V�[��
class SceneGame: public Scene
{
public:
	SceneGame() {};
	~SceneGame() override{};

	// ������
	void Initialize() override;
	// �I����
	void Finalize() override;
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render() override;
	// GUI�`��
	void DrawGUI() override;
	//�f�o�b�O�pGUI�`��
	void DrawDebugGUI() {};
	//�v���C���[�̏o��
	void PopPlayer();
private:
	//Stage* stage = nullptr;

	//Player* player = nullptr;

	CameraController* cameraController = nullptr;

	DirectX::XMFLOAT3 target = { 0,0,0 };
};
