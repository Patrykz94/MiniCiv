#include "Math.h"

float math::lerp(float a, float b, float f)
{
	return (1 - f)*a + f * b;
}
