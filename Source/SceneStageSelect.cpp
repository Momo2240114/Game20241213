#include "SceneStageSelect.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "Stage.h"
#include "System/Input.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;

void SceneStageSelect::Initialize()
{
	sprite = new Sprite("Data/Sprite/StageT.png");
	LoadThumbnailImages();
}

void SceneStageSelect::Finalize()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
	for (int i = sprites.size() - 1; i >= 0; --i) {
		sprites.erase(sprites.begin() + i);
	}
	sprites.clear();
}

void SceneStageSelect::Update(float elapsedTime)
{
	PageCool -= elapsedTime;
	Input& input = Input::Instance();

		int mouseX = input.GetMouse().GetPositionX();
		int mouseY = input.GetMouse().GetPositionY();
		if (input.GetMouse().GetButton() & Mouse::BTN_LEFT)
		{
		// �T���l�C���͈͓̔��Ƀ}�E�X�����邩����
		for (int i = 0; i < LevelHeight; i++)
		{
			for (int j = 0; j < LevelWide; j++)
			{
				if (PageCool <= 0) {
					PageCool = 0;
					// ���[�N���b�N�őO�̃y�[�W
					if (mouseX < 100 && CurrentPage > 0) {

						CurrentPage--;
						SwipeOffset = -1820; // ��ʕ����̃X���C�v
						PageCool = 0.5f;
						IsSwiping = true;
					}
					// �E�[�N���b�N�Ŏ��̃y�[�W
					else if (mouseX > 1820 && CurrentPage < TotalPages - 1) {
						CurrentPage++;
						SwipeOffset = 1820; // ��ʕ����̃X���C�v
						PageCool = 0.5f;
						IsSwiping = true;
					}
					// �������E�c�����̗]�����ϓ��Ɍv�Z
					int horizontalSpacing = (SCREEN_WIDTH - LevelWide * thumbnailWidth) / (LevelWide + 1);
					int verticalSpacing = (SCREEN_HEIGHT - LevelHeight * thumbnailHeight) / (LevelHeight + 1);

					// �T���l�C���̈ʒu�ƃT�C�Y
					int thumbnailX = horizontalSpacing * (j + 1) + j * thumbnailWidth;
					int thumbnailY = verticalSpacing + i * thumbnailHeight * 2;

					// �}�E�X���͈͓��Ȃ� Lev ��ݒ�
					if (mouseX >= thumbnailX && mouseX <= thumbnailX + thumbnailWidth &&
						mouseY >= thumbnailY && mouseY <= thumbnailY + thumbnailHeight)
					{
						// ���x���̃C���f�b�N�X���y�[�W�Ɋ�Â��Čv�Z
						Lev = (CurrentPage * LevelsPerPage) + (i * LevelWide + j);

						Lev;
						PageCool = 1.0f;
					}
				}
			}
		}
	}
		// �X���C�v�A�j���[�V�����̍X�V
		if (IsSwiping)
		{
			if (SwipeOffset > 0)
			{
				SwipeOffset -= SwipeSpeed; // ���փX���C�v
				if (SwipeOffset <= 0) IsSwiping = false;
			}
			else if (SwipeOffset < 0)
			{
				SwipeOffset += SwipeSpeed; // �E�փX���C�v
				if (SwipeOffset >= 0) IsSwiping = false;
			}
		}
	if (IsKeyPressed(VK_RETURN)) { // Enter�L�[�������ꂽ��
		Lev = 0;//���̓X�e�[�W�̔z������f����������Ȃ�����
		Stage::Instance().SetstageLevel(Lev);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}


void SceneStageSelect::UpdatePageOffset(float elapsedTime)
{
	if (swipeTime < 0.5f)
	{
		swipeTime += elapsedTime;
		SwipeOffset = (swipeTime / 0.5f) * SCREEN_WIDTH; // �X���C�v�ʒu���v�Z
	}
	else
	{
		SwipeOffset = 0; // �X���C�v������ɃI�t�Z�b�g�����Z�b�g
	}
}

bool SceneStageSelect::IsMouseOnRightEdge()
{
	Input& input = Input::Instance();
	return input.GetMouse().GetPositionX() > SCREEN_WIDTH - 50; // ��ʉE�[50px
}

bool SceneStageSelect::IsMouseOnLeftEdge()
{
	Input& input = Input::Instance();
	return input.GetMouse().GetPositionX() < 50; // ��ʍ��[50px
}

void SceneStageSelect::LoadThumbnailImages()
{
	// �X�v���C�g�̓ǂݍ��݁i��j
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_1.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_2.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_3.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_4.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_5.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_6.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_7.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_8.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_9.png"));
}


void SceneStageSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderstate = graphics.GetRenderState();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderstate;



	Input& input = Input::Instance();
	int mouseX = input.GetMouse().GetPositionX();
	int mouseY = input.GetMouse().GetPositionY();


	// �������E�c�����̗]�����ϓ��Ɍv�Z
	int horizontalSpacing = (SCREEN_WIDTH - LevelWide * thumbnailWidth) / (LevelWide + 1);
	int verticalSpacing = (SCREEN_HEIGHT - LevelHeight * thumbnailHeight) / (LevelHeight + 1);

	// ���݃y�[�W�Ɨאڃy�[�W�̃T���l�C����`��i�X���C�v�p�j
	for (int page = CurrentPage - 1; page <= CurrentPage + 1; page++)
	{
		if (page < 0 || page >= TotalPages) continue; // �͈͊O�̃y�[�W�̓X�L�b�v

		// �y�[�W�I�t�Z�b�g���v�Z
		float pageOffset = (page - CurrentPage) * SCREEN_WIDTH + SwipeOffset;

		// �y�[�W���̃T���l�C����`��
		for (int i = 0; i < LevelHeight; i++)
		{
			for (int j = 0; j < LevelWide; j++)
			{
				int index = i * LevelWide + j + page * LevelsPerPage;
				if (index >= TotalLevels) break;

				// �T���l�C���̕`��ʒu
				int thumbnailX = horizontalSpacing + j * (thumbnailWidth + horizontalSpacing) + static_cast<int>(pageOffset);
				int thumbnailY = verticalSpacing + i * (thumbnailHeight + verticalSpacing);

				// �}�E�X���T���l�C���͈͓��ɂ��邩���m�F
				bool isMouseOver = (mouseX >= thumbnailX && mouseX <= thumbnailX + thumbnailWidth &&
					mouseY >= thumbnailY && mouseY <= thumbnailY + thumbnailHeight);

				// ���x���̃C���f�b�N�X���y�[�W�Ɋ�Â��Čv�Z
				int L = (CurrentPage * LevelsPerPage) + (i * LevelWide + j);
				//L %=LevelsPerPage;
				L %= 9;
				// �T���l�C���`��
				if (isMouseOver && !(IsSwiping))
				{
					sprites[L].Render(rc,
						thumbnailX - (thumbnailBigWidth - thumbnailWidth) / 2,
						thumbnailY - (thumbnailBigHeight - thumbnailHeight) / 2,
						0,
						thumbnailBigWidth, thumbnailBigHeight, // �T�C�Y��300x300��
						0,
						1, 1, 1, 1); // �X�P�[���Ȃǒ���
				}
				else
				{
					sprites[L].Render(rc,
						thumbnailX, thumbnailY, 0,
						thumbnailWidth, thumbnailHeight,
						0,
						1, 1, 1, 1);
				}
			}
		}
	}
}