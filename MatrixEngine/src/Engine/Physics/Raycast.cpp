#include "Raycast.h"
#include "GameObject\GameObject.h"

#include "../GLM/gtx/perpendicular.hpp"
#include "../GLM/gtx/matrix_transform_2d.hpp"
#include "../GLM/gtx/string_cast.hpp"


// @TODO
//! ---------------------------------------------------------------------------
// \fn		RayCastLine
// \brief	Performs a raycast test against a circle.
//
// \param origin	Origin of the ray
// \param outRes	Direction of the ray (not necessarily normalized).
// \param line		Line segment to raycast against.
// \param outPi		if not NULL, compute the point of intersection and store
//					in this variable such that outPi = origin + dir * ti;
//					where ti is the time of intersection. 
//			
// \details			Do not assume that dir is normalized. You should not
//					modify dir, nor normalize it. 
// 
// \return	the time of intersection as a factor of dir (if dir is normalized, then 
//			this is the same as the distance.
// ---------------------------------------------------------------------------
f32 RayCastLine(const glm::vec2 & origin, const glm::vec2 & dir, const LineSegment2D & line, glm::vec2 * outPi)
{
	//compute the factor of dir
	float ti = (line.mNdotP0 - glm::dot(origin, line.mN)) / (glm::dot(dir, line.mN));

	glm::vec2 Pi = origin + (dir * ti);

	//check if its in the line or not
	if (glm::dot((line.mP1 - line.mP0), (Pi - line.mP0)) < 0 || glm::dot((line.mP0 - line.mP1), (Pi - line.mP1)) < 0)
	{
		outPi = nullptr;
		return -1;
	}

	if (outPi != nullptr)
	{
		//get the intersection point
		*outPi = Pi;
	}

	return ti;
}

// @TODO
//! ---------------------------------------------------------------------------
// \fn		RayCastRect
// \brief	Performs a raycast test against an obb.
//
// \param origin	Origin of the ray
// \param outRes	Direction of the ray (not necessarily normalized).
// \param line		Line segment to raycast against.
// \param outPi		if not NULL, compute the point of intersection and store
//					in this variable such that outPi = origin + dir * ti;
//					where ti is the time of intersection. 
//			
// \details			-Do not assume that dir is normalized. You should not
//					modify dir, nor normalize it. 
//					- This function should use the RayCastLine to find the
//					whether there's an intersection with ray and the obb.
//					- choose minimum time of intersection if the ray
//					intersects multiple edges of the obb (it will).
// 
// \return	the time of intersection as a factor of dir (if dir is normalized, then 
//			this is the same as the distance.
// ---------------------------------------------------------------------------
f32 RayCastRect(const glm::vec2 & origin, const glm::vec2 & dir, Transform2D rect, glm::vec2 * outPi)
{
	glm::vec2 width(abs(rect.mScale.x) / 2, 0);
	glm::vec2 height(0, abs(rect.mScale.y) / 2);

	glm::mat3x3 rotMatrix = glm::rotate(glm::mat3x3(), rect.GetRotation());

	glm::vec2 axisAx(rotMatrix * glm::vec3(glm::vec2(rect.mScale.x / 2, 0), 0));
	glm::vec2 axisAy(rotMatrix * glm::vec3(glm::vec2(0, rect.mScale.y / 2), 0));

	glm::vec2 pt1 = glm::vec2(rotMatrix * glm::vec3(glm::vec2(-abs(rect.mScale.x) / 2, +abs(rect.mScale.y) / 2), 0)) + rect.mPosition;//top left
	glm::vec2 pt2 = glm::vec2(rotMatrix * glm::vec3(glm::vec2(-abs(rect.mScale.x) / 2, -abs(rect.mScale.y) / 2), 0)) + rect.mPosition;//bot left
	glm::vec2 pt3 = glm::vec2(rotMatrix * glm::vec3(glm::vec2(+abs(rect.mScale.x) / 2, -abs(rect.mScale.y) / 2), 0)) + rect.mPosition;//bot right
	glm::vec2 pt4 = glm::vec2(rotMatrix * glm::vec3(glm::vec2(+abs(rect.mScale.x) / 2, +abs(rect.mScale.y) / 2), 0)) + rect.mPosition;//top right

	LineSegment2D line1;
	LineSegment2D line2;
	LineSegment2D line3;
	LineSegment2D line4;

	//build the lines
	BuildLineSegment2D(line1, pt1, pt2); //left line
	BuildLineSegment2D(line2, pt3, pt4); //right line
	BuildLineSegment2D(line3, pt1, pt4); //top line
	BuildLineSegment2D(line4, pt2, pt3); //bot line

	//by default, the min distance is the max.
	float minDistance = FLT_MAX;

	glm::vec2 copy = *outPi;
	float newMin = RayCastLine(origin, dir, line1, &copy);
	//check all distances and get the minimum distance all the time
	if ( newMin < minDistance && newMin != -1) //left line
	{
		*outPi = copy;
		minDistance = newMin;
	}
	
	newMin = RayCastLine(origin, dir, line2, &copy);

	//check all distances and get the minimum distance all the time
	if (newMin < minDistance && newMin != -1) //left line
	{
		*outPi = copy;
		minDistance = newMin;
	}

	newMin = RayCastLine(origin, dir, line3, &copy);
	//check all distances and get the minimum distance all the time
	if (newMin < minDistance && newMin != -1) //left line
	{
		*outPi = copy;
		minDistance = newMin;
	}

	newMin = RayCastLine(origin, dir, line4, &copy);
	//check all distances and get the minimum distance all the time
	if (newMin < minDistance && newMin != -1) //left line
	{
		*outPi = copy;
		minDistance = newMin;
	}

	if (minDistance == FLT_MAX)
		minDistance = -1;

	return minDistance;
}

// @TODO
//! ---------------------------------------------------------------------------
// \fn		RayCastCircle
// \brief	Performs a raycast test against a circle.
//
// \param origin	Origin of the ray
// \param outRes	Direction of the ray (not necessarily normalized).
// \param circle	Circle center
// \param radius	Circle radius.
// \param outPi		if not NULL, compute the point of intersection and store
//					in this variable such that outPi = origin + dir * ti;
//					where ti is the time of intersection. 
//			
// \details			Do not assume that dir is normalized. You should not
//					modify dir, nor normalize it. 
// 
// \return	the time of intersection as a factor of dir (if dir is normalized, then 
//			this is the same as the distance.
// ---------------------------------------------------------------------------
f32 RayCastCircle(const glm::vec2 & origin, const glm::vec2 & dir, const glm::vec2 & circle, f32 radius, glm::vec2 * outPi)
{
	//def. value
	float ti = -1;

	//save the values for the quadratic equation
	float A = glm::dot(dir, dir);
	float B = -2 * glm::dot((circle - origin),(dir));
	float C = glm::dot((circle - origin),(circle - origin)) - (radius*radius);

	//value saved for later
	float delta = (B*B) - (4 * A*C);

	//if there are no intersections
	if (delta < 0)
		return ti;
	
	//if there is only one intersection
	else if (delta == 0)
		ti = -B / (2*A);

	//save the minimum distance between the two intersections
	else if(delta > 0)
		ti = glm::min(((-B + sqrt(delta)) / 2 * A), ((-B - sqrt(delta)) / 2 * A));

	//if there is a point of intersection compute it
	if(ti != -1)
		*outPi = origin + (dir * ti);
	
	glm::vec2 copy = origin;
	float d = glm::distance(copy, (circle));

	//if the squared distance between the two poins is lesser than the squared radius
	if (d <= radius)
	{
		//MINE: this is wrong.
		*outPi = origin + (dir * radius);
		ti = (d-radius);
	}

	return ti;
}


// @PROVIDED
//! ---------------------------------------------------------------------------
// \fn		RayCastCircle
// \brief	Performs a raycast test against a circle. Simple wrapper around
//			the function above
// ---------------------------------------------------------------------------
f32 RayCastCircle(const Ray & ray, const glm::vec2 & circle, f32 radius, glm::vec2 * outPi)
{
	return RayCastCircle(ray.mOrigin, ray.mDirection, circle, radius, outPi);
}

// @PROVIDED
//! ---------------------------------------------------------------------------
// \fn		RayCastRect
// \brief	Performs a raycast test against an obb. Simple wrapper around
//			the function above
// ---------------------------------------------------------------------------
f32 RayCastRect(const Ray & ray, Collider &rect, glm::vec2 * outPi)
{
	return RayCastRect(ray.mOrigin, ray.mDirection, rect.getTransform(), outPi);
}

// @PROVIDED
//! ---------------------------------------------------------------------------
// \fn		RayCastLine
// \brief	Performs a raycast test against a line. Simple wrapper around
//			the function above
// ---------------------------------------------------------------------------
f32 RayCastLine(const Ray & ray, const LineSegment2D & line, glm::vec2 * outPi)
{
	return RayCastLine(ray.mOrigin, ray.mDirection, line, outPi);
}