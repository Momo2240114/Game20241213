#include "PoseRender.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;

void PoseRender::Render(RenderContext rc)
{
	Pose->Render(rc,
		0, 0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 1, 1, 1, 1
	);
}
