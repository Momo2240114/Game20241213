#include "MathUtiles.h"
#include <stdlib.h>
//�w��͈͓��̃����_���Ȓl���v�Z
float MathUtiles::RandomRange(float min, float max)
{
	//�O�`�P�܂ł̃����_���Ȓl
	float value = static_cast<float>(rand()) / RAND_MAX;

	return min + (max - min) * value;
}
