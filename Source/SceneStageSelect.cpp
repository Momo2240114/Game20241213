#include "SceneStageSelect.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "Stage.h"
#include "System/Input.h"
#include "KeyPut.h"
#include "StageSave.h"
#include "ScoreRender.h"
#include "AudioManager.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;

void SceneStageSelect::Initialize()
{
	LoadThumbnailImages();
	ScoreRender::Instance().init();
	AudioManager::Instance().PlayBGM(BGM_Number::Selet);
}

void SceneStageSelect::Finalize()
{
	for (int i = sprites.size() - 1; i >= 0; --i) {
		sprites.erase(sprites.begin() + i);
	}
	sprites.clear();
}

void SceneStageSelect::Update(float elapsedTime)
{
	Input& input = Input::Instance();
	bool Left = input.GetMouse().GetButtonDown() & Mouse::BTN_LEFT;
	bool Right = input.GetMouse().GetButtonDown() & Mouse::BTN_RIGHT;

	PageCool -= elapsedTime;
	ScoreRender::Instance().UpdateMoveY(elapsedTime);

	if (IsScore && PageCool < 0)
	{
		if (Right)
		{

			IsScore = false;
			PageCool = 0.2f;
		}
	}

	else {
		Select(elapsedTime);
	}
}


void SceneStageSelect::UpdatePageOffset(float elapsedTime)
{
	if (swipeTime < 0.5f)
	{
		swipeTime += elapsedTime;
		SwipeOffset = (swipeTime / 0.5f) * SCREEN_WIDTH; // スワイプ位置を計算
	}
	else
	{
		SwipeOffset = 0; // スワイプ完了後にオフセットをリセット
	}
}

void SceneStageSelect::LoadThumbnailImages()
{
	// スプライトの読み込み（例）
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_1.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_2.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_3.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_4.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_5.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_6.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_7.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_8.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_9.png"));	
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_1.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_2.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_3.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_4.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_5.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_6.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_7.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_8.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_9.png"));	
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_1.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_2.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_3.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_4.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_5.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_6.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_7.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_8.png"));
	sprites.push_back(Sprite("Data/Sprite/Thumbnail_9.png"));	
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


	// 横方向・縦方向の余白を均等に計算
	int horizontalSpacing = (SCREEN_WIDTH - LevelWide * thumbnailWidth) / (LevelWide + 1);
	int verticalSpacing = (SCREEN_HEIGHT - LevelHeight * thumbnailHeight) / (LevelHeight + 1);

	// 現在ページと隣接ページのサムネイルを描画（スワイプ用）
	for (int page = CurrentPage - 1; page <= CurrentPage + 1; page++)
	{
		if (page < 0 || page >= TotalPages) continue; // 範囲外のページはスキップ

		// ページオフセットを計算
		float pageOffset = (page - CurrentPage) * SCREEN_WIDTH + SwipeOffset;

		// ページ内のサムネイルを描画
		for (int i = 0; i < LevelHeight; i++)
		{
			for (int j = 0; j < LevelWide; j++)
			{
				int index = i * LevelWide + j + page * LevelsPerPage;
				if (index >= TotalLevels) break;

				// サムネイルの描画位置
				int thumbnailX = horizontalSpacing + j * (thumbnailWidth + horizontalSpacing) + static_cast<int>(pageOffset);
				int thumbnailY = verticalSpacing + i * (thumbnailHeight + verticalSpacing);

				// マウスがサムネイル範囲内にあるかを確認
				bool isMouseOver = (mouseX >= thumbnailX && mouseX <= thumbnailX + thumbnailWidth &&
					mouseY >= thumbnailY && mouseY <= thumbnailY + thumbnailHeight);

				// レベルのインデックスをページに基づいて計算
				int L = (page * LevelsPerPage) + (i * LevelWide + j);
	 

				// サムネイル描画
				if (isMouseOver && !(IsSwiping))
				{
					sprites[L].Render(rc,
						thumbnailX - (thumbnailBigWidth - thumbnailWidth) / 2,
						thumbnailY - (thumbnailBigHeight - thumbnailHeight) / 2,
						0,
						thumbnailBigWidth, thumbnailBigHeight, // サイズを拡大に
						0,
						1, 1, 1, 1); // スケールなど調整
				
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
	if (IsScore)
	{
		ScoreRender::Instance().Render(Lev,rc);
	}
}

void SceneStageSelect::Select(float elapsedTime)
{
	if (!(IsSwiping)) {

		Input& input = Input::Instance();
		bool Left = input.GetMouse().GetButtonDown() & Mouse::BTN_LEFT;
		bool Right = input.GetMouse().GetButtonDown() & Mouse::BTN_RIGHT;

		int mouseX = input.GetMouse().GetPositionX();
		int mouseY = input.GetMouse().GetPositionY();
		
		// サムネイルの範囲内にマウスがあるか判定
		for (int i = 0; i < LevelHeight; i++)
		{
			for (int j = 0; j < LevelWide; j++)
			{
				if (PageCool <= 0) {
					PageCool = 0;
					// 左端クリックで前のページ
					if ((((mouseX < 100 && Left) || KeyPressed(0x41) || KeyPressed(0x25))
						&& CurrentPage > 0))
					{
						CurrentPage--;
						SwipeOffset = -1820; // 画面幅分のスワイプ
						PageCool = 0.4f;
						IsSwiping = true;
					}
					// 右端クリックで次のページ
					else if ((((mouseX > 1820 && Left) || KeyPressed(0x44) || KeyPressed(0x27))
						&& CurrentPage < TotalPages - 1)
						) {
						
						CurrentPage++;
						SwipeOffset = 1820; // 画面幅分のスワイプ
						PageCool = 0.4f;
						IsSwiping = true;
					}

				}
				// 横方向・縦方向の余白を均等に計算
				int horizontalSpacing = (SCREEN_WIDTH - LevelWide * thumbnailWidth) / (LevelWide + 1);
				int verticalSpacing = (SCREEN_HEIGHT - LevelHeight * thumbnailHeight) / (LevelHeight + 1);

				// サムネイルの位置とサイズ
				int thumbnailX = horizontalSpacing * (j + 1) + j * thumbnailWidth;
				int thumbnailY = verticalSpacing + i * thumbnailHeight * 2;

				// マウスが範囲内なら Lev を設定
				if (mouseX >= thumbnailX && mouseX <= thumbnailX + thumbnailWidth &&
					mouseY >= thumbnailY && mouseY <= thumbnailY + thumbnailHeight)
				{
					// レベルのインデックスをページに基づいて計算
					Lev = (CurrentPage * LevelsPerPage) + (i * LevelWide + j);
					Lev;
					if (PageCool <= 0)
					{
						if (Right) {
							ScoreRender::Instance().ResetMoveY();
							IsScore = true;
							PageCool = 0.1f;
						}
					}

					if ((KeyPressedIn(VK_RETURN) || Left)) // Enterキーもしくわ左クリックが押されたら
					{
						AudioManager::Instance().StopBGM();
						if (Lev > 1)Lev = 1;
						Stage::Instance().SetstageLevel(Lev);
						SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
					}
				}
			}
		}
	

	}
	// スワイプアニメーションの更新
	if (IsSwiping)
	{
		if (SwipeOffset > 0)
		{
			SwipeOffset -= SwipeSpeed; // 左へスワイプ
			if (SwipeOffset <= 0) IsSwiping = false;
		}
		else if (SwipeOffset < 0)
		{
			SwipeOffset += SwipeSpeed; // 右へスワイプ
			if (SwipeOffset >= 0) IsSwiping = false;
		}
	}

}
