#pragma once

#include "StageSave.h"
#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

class ScoreRender
{
public:
	static ScoreRender& Instance() {
		static ScoreRender Ins;
		return Ins;
	}
	void init();
	void Render(int L, RenderContext rc);
	void Time(RenderContext rc,float Time);
	void Cost(RenderContext rc,int Cost);
	void Level(RenderContext rc,int L);
	void RenderNumber(RenderContext rc, int number, int xOffsetBase, int yOffset, int maxDigits);
	void ResetMoveY();
	void ScoreRender::UpdateMoveY(float deltaTime);
	
private:
	Sprite* Back = nullptr;
	Sprite* IsClear = nullptr;
	Sprite* Number = nullptr;

	const float NumberSizeX = 300;
	const float NumberSizeY = 300;

	float CToffsetX = 500;
	float MoveY = -1800;
};

