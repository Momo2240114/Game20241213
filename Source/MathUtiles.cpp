#include "MathUtiles.h"
#include <stdlib.h>
//指定範囲内のランダムな値を計算
float MathUtiles::RandomRange(float min, float max)
{
	//０～１までのランダムな値
	float value = static_cast<float>(rand()) / RAND_MAX;

	return min + (max - min) * value;
}
