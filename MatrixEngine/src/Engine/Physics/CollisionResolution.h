/*****************************************************************/
/*Author: Joaquin Bilbao										 */
/*Brief description: Contains prototypes for collision resolution*/
/*****************************************************************/
#ifndef COLLISION_RESOLUTION_H
#define COLLISION_RESOLUTION_H
// ----------------------------------------------------------------------------

#include "../GLM/glm.hpp"

class GameObject;
class Transform2D;

struct Contact
{
	//Xo = mPi;
	//X1 = mPi + mNormal * mPenetration;

	//point of intersection
	glm::vec2 mPi;
	//normal of the intersection
	glm::vec2 mNormal;
	//penetration of the object
	float	mPenetration;
};

//! ---------------------------------------------------------------------------
// \fn		StaticCircleToStaticCircleEx
// \brief	Checks overlap between two circles. If an overla exists, it stores
//			the contact data into pResult.
//
// \details	
//			- You must compute ALL the contact data.
//			- You should check for pResult not being equal to NULL.
//			- Normal must be from Circle1 to Circle2.
//			- Point of intersection must be on the edge of Circle1. It represents
//			  The point where both circles touch (after collision is resolved).
// 
//  \return	true if the shapes overlap, false otherwise
// ---------------------------------------------------------------------------
bool StaticCircleToStaticCircleEx(const glm::vec2 & Center1, float Radius1, const glm::vec2 & Center2, float Radius2, Contact * pResult);

//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticCircleEx
// \brief	Checks overlap between a rectangle and a circle.If an overlap exists, 
// 			it stores the contact data into pResult.
//
// \details	
//			- You must handle the case when the circle is inside the rectangle
//			  as described in class. 
//			- You should check for pResult not being equal to NULL.
//			- You must compute ALL the contact data.
//			- You must assume that the contact viewer is the rectangle. 
//			  (i.e. normal goes from Box to Circle).
// 
//  \return	true if the shapes overlap, false otherwise
// ---------------------------------------------------------------------------
bool StaticRectToStaticCircleEx(const glm::vec2 & rectPos, float rectWidth, float rectHeight, const glm::vec2 & circlePos, float radius, Contact * pResult);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticOBBToStaticCircleEx
// \brief	Checks overlap between and oriented box and a circle.If an overlap exists, 
// 			it stores the contact data into pResult.
//
// \details	
//			- You should use StaticrectToStaticCircleEx.
//			- You must compute ALL the contact data.
//			- You should check for pResult not being equal to NULL.
//			- You must assume that the contact viewer is the OBB. (i.e. normal goes from 
//			  Box to Circle).
// 
//  \return	true if the shapes overlap, false otherwise
// ---------------------------------------------------------------------------
bool StaticOBBToStaticCircleEx(Transform2D & OBB, const glm::vec2 & Center, float Radius, Contact * pResult);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticRectEx
// \brief	Checks overlap between two AABB , using the simplified Separating Axis
// 			Theorem described in class. If an overlap exists, it stores the 
// 			contact data into pResult.
//
// \details	
//			- You must compute ALL the contact data.
//			- You should check for pResult not being equal to NULL.
//			- To determine the point of collision, use the following logic:
//				* check if any corner of box2 is inside of box1, if so use 
//				  it as the point of interesection. 
//				* otherwise, check if any corner of box1 is inside of box2, use
//				  it as the point of intersection.
// 
//  \return	true if the shapes overlap, false otherwise
// ---------------------------------------------------------------------------
bool StaticRectToStaticRectEx(const glm::vec2 &pos1, const glm::vec2 &size1, const glm::vec2 &pos2, const glm::vec2 &size2, Contact * pResult);

// @TODO
//! ---------------------------------------------------------------------------
// \fn		OrientedRectToOrientedRectEx
// \brief	Checks overlap between two oriented box, using the Separating Axis
// 			Theorem described in class. If an overlap exists, it stores the 
// 			contact data into pResult.
//
// \details	
//			- You must compute ALL the contact data.
//			- You should check for pResult not being equal to NULL
// 
//  \return	true if the shapes overlap, false otherwise
// ---------------------------------------------------------------------------
bool OrientedRectToOrientedRectEx(Transform2D & OBB1, Transform2D & OBB2, Contact * pResult);


#endif
