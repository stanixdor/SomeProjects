#ifndef RAYCAST_H_
#define RAYCAST_H_
#define GLM_FORCE_RADIANS
#include "Physics\LineSegment2D.h"
#include "Collider.h"
#include "Transform\Transform2D.h"

struct Ray
{
	glm::vec2 mOrigin;
	glm::vec2 mDirection;
};
f32 RayCastLine(const Ray & ray, const LineSegment2D & line, glm::vec2 * outPi);
f32 RayCastLine(const glm::vec2 & origin, const glm::vec2 & dir, const LineSegment2D & line, glm::vec2 * outPi);

f32 RayCastRect(const Ray & ray, Collider &rect, glm::vec2 * outPi);
f32 RayCastRect(const glm::vec2 & origin, const glm::vec2 & dir, Transform2D & rect, glm::vec2 * outPi);

f32 RayCastCircle(const Ray & ray, const glm::vec2 & circle, f32 radius, glm::vec2 * outPi);
f32 RayCastCircle(const glm::vec2 & origin, const glm::vec2 & dir, const glm::vec2 & circle, f32 radius, glm::vec2 * outPi);

#endif