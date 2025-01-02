#pragma once
#include "Stage.h"
//#include "Player.h"
#include "CameraController.h"
#include "Scene.h"

// ゲームシーン
class SceneGame: public Scene
{
public:
	SceneGame() {};
	~SceneGame() override{};

	// 初期化
	void Initialize() override;
	// 終了化
	void Finalize() override;
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render() override;
	// GUI描画
	void DrawGUI() override;
	//デバッグ用GUI描画
	void DrawDebugGUI() {};
	//プレイヤーの出現
	void PopPlayer();
private:
	//Stage* stage = nullptr;

	//Player* player = nullptr;

	CameraController* cameraController = nullptr;

	DirectX::XMFLOAT3 target = { 0,0,0 };
};
