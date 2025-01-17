#include "ScoreRender.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;


void ScoreRender::RenderNumber(RenderContext rc, int number, int xOffsetBase, int yOffset, int maxDigits)
{
    // �������v�Z
    int digitCount = 0;
    int temp = number;
    do {
        digitCount++;
        temp /= 10;
    } while (temp > 0);

    // �ő包���𒴂���ꍇ���l��
    digitCount = min(digitCount, maxDigits);

    // �s�����Ă��錅����0�Ŗ��߂�
    for (int i = digitCount; i < maxDigits; i++) {
        Number->Render(rc,
            xOffsetBase + (maxDigits - i - 1) * 200, yOffset + MoveY, 0,  // �\���ʒu
            200, 200, 0, 0,  // �T�C�Y�Ɛ؂蔲���ʒu
            NumberSizeX, NumberSizeY,  // �؂蔲���T�C�Y
            0, 1, 1, 1, 1  // �F�ⓧ���x�ݒ�
        );
    }

    // �`��J�n�ʒu�𒲐� (�E�l�ߑΉ�)
    int xOffset = xOffsetBase + (maxDigits - digitCount) * 200;

    // �e����`�� (�傫�������珇��)
    temp = number;
    for (int i = 0; i < digitCount; i++) {
        int digit = temp / static_cast<int>(pow(10, digitCount - i - 1)); // �e���̒l
        temp %= static_cast<int>(pow(10, digitCount - i - 1));            // �c��̐��l

        Number->Render(rc,
            xOffset, yOffset + MoveY, 0,          // �\���ʒu
            200, 200,                     // �\���T�C�Y
            digit * NumberSizeX, 0,       // �؂蔲���ʒu
            NumberSizeX, NumberSizeY,     // �؂蔲���T�C�Y
            0, 1, 1, 1, 1                 // �F�ⓧ���x�ݒ�
        );
        xOffset += 200; // ���̌��ɐi��
    }
}

void ScoreRender::ResetMoveY()
{
    MoveY = -1800;
}

void ScoreRender::UpdateMoveY(float deltaTime)
{
    // deltaTime�����Y���W�𑝉��i��: 1�b�Ԃ�200�s�N�Z���~���j
    MoveY += 2000.0f * deltaTime;

    // ��ʉ��ɍs���߂����烊�Z�b�g�i�K�v�ɉ����ĕύX�j
    if (MoveY > 0) {
        MoveY = 0;
    }
}

void ScoreRender::init()
{
	Back = new Sprite("Data/Sprite/ScoreBack.png");
	IsClear = new Sprite("Data/Sprite/IsCleared.png");
	Number = new Sprite("Data/Sprite/Number.png");

    StageSaveManager::Instance().Load();
}

void ScoreRender::Render(int L, RenderContext rc)
{
	Back->Render(rc,
		0, 0 + MoveY, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0,
		1, 1, 1, 1
	);
    // �X�e�[�W�f�[�^�̎擾
    auto stageData = StageSaveManager::Instance().GetStageData(L);

    Level(rc, L + 1);
   
    // �N���A�󋵂̕`��
    if (stageData.isCleared) {
        // �N���A���Ă���ꍇ
        IsClear->Render(rc,
            1150, 300 + MoveY, 0, // �\���ʒu
            200, 200, // �\���T�C�Y
            300, 0, // �؂蔲���ʒu (x=0, y=0)
            300, 300, // �؂蔲���T�C�Y
            0, 1, 1, 1, 1
        );
    }
    else {
        // ���N���A�̏ꍇ
        IsClear->Render(rc,
            1150, 300 + MoveY, 0, // �\���ʒu
            200, 200, // �\���T�C�Y
            0, 0, // �؂蔲���ʒu (x=0, y=0)
            300, 300, // �؂蔲���T�C�Y
            0, 1, 1, 1, 1
        );
    }
    if (stageData.isCleared) {

        Time(rc, stageData.shortestTime);
        Cost(rc, stageData.lowestCost);
    }
}

void ScoreRender::Time(RenderContext rc, float Time)
{
    // ���Ԃ̐��������̂ݎ擾
    int timeInt = static_cast<int>(Time);

    // �ő�4���̐�������`��
    RenderNumber(rc, timeInt, 850, 575, 4);
}

void ScoreRender::Cost(RenderContext rc, int Cost)
{
    // �ŏ��R�X�g�̕`�� (�ő�4��)
    RenderNumber(rc, Cost, 850, 810, 4);
}

void ScoreRender::Level(RenderContext rc, int L)
{
    // ���x���̕`�� (�ő�4��)
    RenderNumber(rc, L, 1100, 0, 2);
}