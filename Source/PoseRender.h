#pragma once

#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"


//ポーズ画面のレンダー
class PoseRender
{
public:
	static PoseRender& Instance()
	{
		static PoseRender ins;
		return ins;
	}
	void Init() {
		Pose = new Sprite("Data/Sprite/Pose.png");
	}

	void Delete() {
		delete Pose;
		Pose = nullptr;
	}

	void Render(RenderContext rc);

private:
	Sprite *Pose;


};

