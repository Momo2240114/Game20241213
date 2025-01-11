#pragma once

#include "Scene.h"
#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

constexpr int TotalLevels = 200;// �S�X�e�[�W�̐�  

//level�Z���N�g
class SceneStageSelect :public Scene
{
public:
    SceneStageSelect() {};
    ~SceneStageSelect() {};
    void Initialize()override;
    void Finalize()override;
    void Update(float elapsedTime)override;
    void Render()override;
    void DrawGUI() override{};

private:
    void UpdatePageOffset(float elapsedTime);
    bool IsMouseOnRightEdge();
    bool IsMouseOnLeftEdge();

    // �T���l�C���摜�����[�h����
    void LoadThumbnailImages();

    //�L�[�������ꂽ�Ƃ��̌��m�p
    bool IsKeyPressed(int key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    int Lev = 0;

    int CurrentPage = 0;
    float PageCool = 0;
    float SwipeOffset = 0.0f;   // �y�[�W�̃I�t�Z�b�g
    float swipeTime = 0.0f;     // �X���C�v�̐i�s��
    float SwipeSpeed = 70;
    float pageOffset = 0;
    // ���x�����̐ݒ�

    const int LevelWide = 3;       // ���ɕ��ׂ�T���l�C����
    const int LevelHeight = 3;     // �c�ɕ��ׂ�T���l�C����
    const int LevelsPerPage = (LevelWide * LevelHeight);  // 1�y�[�W�ɕ\�����郌�x����
    const int TotalPages = (TotalLevels + LevelsPerPage - 1) / LevelsPerPage; // ���y�[�W��

    bool IsSwiping = false;

    // �T���l�C���̒ʏ�T�C�Y
    const  int thumbnailWidth = 256;
    const  int thumbnailHeight = 256;

    // �T���l�C���̊g�厞�̕��ƍ���
    const int thumbnailBigWidth = 300;  // ��
    const int thumbnailBigHeight = 300; // ����

    Sprite* sprite = nullptr;

    std::vector<Sprite> sprites; // �X�v���C�g�̔z��

};
