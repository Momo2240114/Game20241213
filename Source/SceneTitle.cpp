#include"System/Graphics.h"
#include"System/Input.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneManager.h"


void SceneTitle::Initialize()
{
	sprite = new Sprite("Data/Sprite/Title.png");
}

void SceneTitle::Finalize()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	const GamePadButton anyButton =
		GamePad::BTN_A |
		GamePad::BTN_B |
		GamePad::BTN_X |
		GamePad::BTN_Y;
	if (gamepad.GetButtonDown() & anyButton)
	{
		SceneManager& sceneManager = SceneManager::Instance();
		/*sceneManager.ChangeScene(new SceneGame());*/
		sceneManager.ChangeScene(new SceneLoading(new SceneGame()));
	
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
