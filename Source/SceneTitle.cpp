#include"System/Graphics.h"
#include"System/Input.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneStageSelect.h"
#include "KeyPut.h"
#include "AudioManager.h"

void SceneTitle::Initialize()
{
	sprite = new Sprite("Data/Sprite/Title.png");
	Ready = false;
	AudioManager::Instance().PlayBGM(BGM_Number::Title);
}

void SceneTitle::Finalize()
{
	AudioManager::Instance().StopBGM();
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void SceneTitle::Update(float elapsedTime)
{
	Input& input = Input::Instance();
	bool Left = input.GetMouse().GetButtonDown() & Mouse::BTN_LEFT;
	bool Right = input.GetMouse().GetButtonDown() & Mouse::BTN_RIGHT;

	int mouseX = input.GetMouse().GetPositionX();
	int mouseY = input.GetMouse().GetPositionY();
	GamePad& gamepad = Input::Instance().GetGamePad();
	const GamePadButton anyButton =
		GamePad::BTN_A |
		GamePad::BTN_B |
		GamePad::BTN_X |
		GamePad::BTN_Y;
	if (gamepad.GetButtonDown() & anyButton || Left || KeyPressed(VK_RETURN))
	{
		Ready = true;
	}
	if (Ready)
	{
		Timer += elapsedTime;
	}
	if(Timer > 0.1f)
	{
		AudioManager::Instance().StopBGM();
		AudioManager::Instance().PlaySE(SE_Number::GoTitleToSelect);
		SceneManager::Instance().ChangeScene(new SceneStageSelect);
	}
}

void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	RenderState* renderstate = graphics.GetRenderState();

	RenderContext rc;
	rc.deviceContext = dc;
	rc.renderState = renderstate;
	//2Dスプライト描画
	{
		//タイトル描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		sprite->Render(rc,
			0, 0, 0,
			screenWidth, screenHeight,
			0,
			1, 1, 1, 1);
	}
}

void SceneTitle::DrawGUI()
{
}
