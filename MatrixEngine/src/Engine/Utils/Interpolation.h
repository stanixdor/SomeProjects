#pragma once

namespace Interpolation
{
	float lerp(float begin, float end, float percentage);

	float QuadraticEaseIn(float currenttime, float start, float cahngeinvalue, float duration);
};