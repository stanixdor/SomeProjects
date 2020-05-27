#include "Interpolation.h"

float Interpolation::lerp(float begin, float end, float percentage)
{
	return (1 - percentage) * begin + percentage * end;
}

float Interpolation::QuadraticEaseIn(float currenttime, float start, float cahngeinvalue, float duration)
{
	currenttime /= duration;
	return cahngeinvalue*currenttime*currenttime + start;
}