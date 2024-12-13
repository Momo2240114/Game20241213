#pragma once
#include "System/Sprite.h"
#include "Scene.h"
#include <thread>

class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* NextScene):nextScene(NextScene) {};
	~SceneLoading()override {};


	void Initialize()override;
	void Finalize()override;
	void Update(float elapsedTime) override;
	void Render()override;
	void DrawGUI() override;
private:
	static void LoadingThread(SceneLoading* Scene);

private:
	Sprite* sprite = nullptr;
	float angle = 0.0f;

	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
};

