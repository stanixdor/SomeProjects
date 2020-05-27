// ---------------------------------------------------------------------------
#ifndef LINE_SEGMENT_2D_H
#define LINE_SEGMENT_2D_H
// ---------------------------------------------------------------------------
#include "GLM\glm.hpp"
#include "Core/DataTypes.h"

struct LineSegment2D
{
	glm::vec2 mP0;		// Point on the line
	glm::vec2 mP1;		// Point on the line
	glm::vec2 mN;			// Line's normal
	float mNdotP0;		// To avoid computing it every time it's needed
};

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
bool BuildLineSegment2D(LineSegment2D& LS, glm::vec2& Point0, glm::vec2& Point1);

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
float StaticPointToStaticLineSegment(glm::vec2& Point, LineSegment2D& LS);

//! ---------------------------------------------------------------------------
// \fn		PointInLineSegments
// \brief	Given a point P and an array of LineSegment2D, determines if a point
//			is contained by all line segments (ie. inside half plane of all segments).
//
// \details	none.
// 
// \return	true if the point is inside all line segments, false otherwise.
// ---------------------------------------------------------------------------
bool PointInLineSegments(glm::vec2& Point, LineSegment2D *LS, u32 count);

// ---------------------------------------------------------------------------
#endif