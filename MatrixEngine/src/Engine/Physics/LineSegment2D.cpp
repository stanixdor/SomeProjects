// ---------------------------------------------------------------------------
#include "LineSegment2D.h"
// ---------------------------------------------------------------------------
#include "GLM/gtx/perpendicular.hpp"
// @TODO
//! ---------------------------------------------------------------------------
// \fn		BuildLineSegment2D
// \brief	This function builds a 2D line segment's data using 2 points
//
// \details:
//			- Edge is defined as E = P1-P0
//			- mN is the outward normal and is defined as mN = E.Perp().Normalize();
//			- mNdotP0 = the dot product of the normal with p0.
// 
// \return	true if the line equation was built successfully
//			false if Point0 and Point1 are the same point. (Sanity check). 
// ---------------------------------------------------------------------------
bool BuildLineSegment2D(LineSegment2D& LS, glm::vec2 &Point0, glm::vec2 &Point1)
{
	//check if its the same point
	if (Point0 == Point1)
		return false;

	//save start and end point
	LS.mP0 = Point0;
	LS.mP1 = Point1;

	glm::vec2 edge = (Point0 - Point1);
	//compute the normal

	//glm::perp(Center2, Center1);
	//glm::normalize(pResult->mNormal);

	LS.mN = glm::perp(Point1, Point0);
	LS.mN = glm::normalize(LS.mN);


	//compute the dot of the normal with P0
	LS.mNdotP0 = glm::dot(LS.mN, LS.mP0);

	return true;
}


// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticPointToStaticLineSegment
// \brief	This function determines the distance separating a point from a line
//
// \details	
// 
// \return	The distance. Note that the returned value should be:
//			- Negative if the point is in the line's inside half plane
//			- Positive if the point is in the line's outside half plane
//			- Zero if the point is on the line
// ---------------------------------------------------------------------------
float StaticPointToStaticLineSegment(glm::vec2&P, LineSegment2D& LS)
{
	//check the distance
	return (glm::dot(P,LS.mN) - (LS.mNdotP0));
}


// @TODO
//! ---------------------------------------------------------------------------
// \fn		PointInLineSegments
// \brief	Given a point P and an array of LineSegment2D, determines if a point
//			is contained by all line segments (ie. inside half plane of all segments).
//
// \details	none.
// 
// \return	true if the point is inside all line segments, false otherwise.
// ---------------------------------------------------------------------------
bool PointInLineSegments(glm::vec2 &P, LineSegment2D *LS, u32 count)
{
	//for every Line Segment
	while (count > 0)
	{
		//check if the distance is positive (then its outside the half plane)
		if (StaticPointToStaticLineSegment(P, *LS) > 0)
			return false;

		//advance to the next line segment and update count
		LS++;
		count--;
	}
	return true;
}