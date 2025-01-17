#pragma once

#include "Scene.h"
#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

constexpr int TotalLevels = 4;// �S�X�e�[�W�̐�  

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
    void Select(float elapsedTime);
private:
    void UpdatePageOffset(float elapsedTime);

    // �T���l�C���摜�����[�h����
    void LoadThumbnailImages();

    int Lev = 0;

    int CurrentPage = 0;
    float PageCool = 0;
    float SwipeOffset = 0.0f;   // �y�[�W�̃I�t�Z�b�g
    float swipeTime = 0.0f;     // �X���C�v�̐i�s��
    float SwipeSpeed = 70;
    float pageOffset = 0;
    bool IsScore = false;
    // ���x�����̐ݒ�

    const int LevelWide = 1;       // ���ɕ��ׂ�T���l�C����
    const int LevelHeight = 1;     // �c�ɕ��ׂ�T���l�C����
    const int LevelsPerPage = (LevelWide * LevelHeight);  // 1�y�[�W�ɕ\�����郌�x����
    const int TotalPages = (TotalLevels + LevelsPerPage - 1) / LevelsPerPage; // ���y�[�W��

    bool IsSwiping = false;

    // �T���l�C���̒ʏ�T�C�Y
    const  int thumbnailWidth = 500;
    const  int thumbnailHeight = 500;

    // �T���l�C���̊g�厞�̕��ƍ���
    const int thumbnailBigWidth = 500;  // ��
    const int thumbnailBigHeight = 500; // ����



    std::vector<Sprite> sprites; // �X�v���C�g�̔z��

};
