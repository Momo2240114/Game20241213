#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
	if (NextScene)
	{
		Clear();
		NowScene = NextScene;
		NextScene = nullptr;

		if (!NowScene->Isready())
		{
			NowScene->Initialize();
		}
	}
	if (NowScene)
	{
		NowScene->Update(elapsedTime);
	}
}

void SceneManager::Render()
{
	if (NowScene)
	{
		NowScene->Render();
	}
}

void SceneManager::DrawGUI()
{
	if (NowScene)
	{
		NowScene->DrawGUI();
	}
}

void SceneManager::Clear()
{
	if (NowScene)
	{
		NowScene->Finalize();
		delete NowScene;
		NowScene = nullptr;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	NextScene = scene;
}
