#include "SceneLoading.h"
#include "SceneManager.h"
#include "System/Graphics.h"
#include "System/Input.h"

void SceneLoading::Initialize()
{

	sprite = new Sprite("Data/Sprite/LoadingIcon.png");

	// スレッド開始
	thread = new std::thread(&SceneLoading::LoadingThread, this);
}

void SceneLoading::Finalize()
{
	// スレッドの終了を待機して解放
	if (thread && thread->joinable()) {
		thread->join(); // スレッドの終了を待機
		delete thread;  // メモリ解放
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

	//次のシーンの準備が完了すれば切り替える
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
	//2Dスプライト描画
	{
		//タイトル描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float sprWidth = 256;
		float sprHeight = 256;
		//float positionX = screenWidth - sprWidth;
		//float positionY = screenHeight - sprHeight;
		float positionX = (screenWidth - sprWidth) / 2; // 中央に配置
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
	// COM関連の初期化
	CoInitialize(nullptr);

	// 次のシーンの初期化
	Scene->nextScene->Initialize();

	// COM関連の解放
	CoUninitialize();

	// 次のシーンの準備完了を設定
	Scene->SetReady();
}
