// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		ContactCollisions.cpp
//	Purpose:		This file contains the functions detect and save the data of a collisions
//	Project:		CS230_joaquin.Bilbao_3
//	Author:			Joaquin Bilbao, Joaquin.Bilbao, 540002415
// ----------------------------------------------------------------------------
#include "Engine/Physics/Collisions.h"
#include "Engine/Physics/CollisionResolution.h"
#include "Engine/Transform/Transform2D.h"
#include "../GLM/gtx/perpendicular.hpp"
#include "../GLM/gtx/matrix_transform_2d.hpp"
#include "../GLM/gtx/string_cast.hpp"
#include <array>	//use arrays from the STL
#include <limits>	// UINT_MAX
#include <cfloat>	// FLT_MAX
#include <iostream>
// ---------------------------------------------------------------------------

// @TODO
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
bool StaticCircleToStaticCircleEx(const glm::vec2 & Center1, float Radius1, const glm::vec2 & Center2, float Radius2, Contact * pResult)
{
	//check if its null
	if (pResult == nullptr)
		return StaticCircleToStaticCircle(Center1, Radius1, Center2, Radius2);

	//distance between two centers
	float distance = glm::distance(Center2, Center1);

	//if the sum of the radius is greater or equal to the distance, the circles collide
	if (Radius1 + Radius2 >= distance)
	{
		//find the normal and normalize it
		pResult->mNormal = glm::perp(Center2, Center1);
		pResult->mNormal = glm::normalize(pResult->mNormal);

		//The penetration is equal to the difference between the sum of the radius and the distance between both centers
		pResult->mPenetration = (Radius1 + Radius2) - distance;

		//the point of intersection is located at the extreme of the center
		pResult->mPi = Center1 + (pResult->mNormal * Radius1);

		return true;
	}
	return false;
}

// @TODO
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
bool StaticRectToStaticCircleEx(const glm::vec2 & Rect, float Width, float Height, const glm::vec2 & Center, float Radius, Contact * pResult)
{
	if (pResult == nullptr)
		return StaticRectToStaticCirlce(Rect, Width, Height, Center, Radius);

	if (!StaticRectToStaticCirlce(Rect, Width, Height, Center, Radius))
		return false;

	int nearestX = static_cast<int>(glm::clamp(Center.x, (Rect.x - Width / 2), (Rect.x + Width / 2))   );
	int nearestY = static_cast<int>(glm::clamp(Center.y, (Rect.y - Height / 2), (Rect.y + Height / 2)) );

	glm::vec2 nearest(nearestX, nearestY);

	float distance = glm::distance(Center, nearest);

	//float displacement = glm::distance(Rect, nearest);

	//get the normal, normalize it
	if (distance == 0)
	{
		//get the vector
		glm::vec2 result = Rect - Center;


		//compute penetrations
		float xPen = glm::length(glm::proj(result, glm::vec2(1, 0)));
		float yPen = glm::length(glm::proj(result, glm::vec2(0, 1)));

		//depending on the penetration set the normal and the pen.
		if (xPen >= yPen)
		{
			pResult->mNormal = glm::vec2(1, 0);
			pResult->mPenetration = (Width / 2) + Radius - xPen;

		}
		else
		{
			pResult->mNormal = glm::vec2(0, 1);
			pResult->mPenetration = (Height / 2) + Radius - yPen;

		}


		//check if we should inverse the penetration
		if (glm::dot(result, pResult->mNormal) > 0)
			pResult->mNormal = -pResult->mNormal;

		//set PI
		pResult->mPi = nearest;
	}
	else
	{
		//get the distance between the nearest and the normal
		pResult->mNormal = (Center - nearest);
		pResult->mNormal = glm::normalize(pResult->mNormal);

		//The penetration is equal to the difference between the sum of the radius and the distance between the closest rectangle
		pResult->mPenetration = Radius - distance;

		//the point of intersection is located at the extreme of the center
		pResult->mPi = nearest;

		return true;
	}

	return true;
}
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
// ---------------------------------------------------------------------------q
bool StaticOBBToStaticCircleEx(Transform2D & OBB, const glm::vec2 & Center, float Radius, Contact * pResult)
{
	
	if (!OrientedRectToStaticCirlce(OBB.GetPos(), OBB.GetScale().x, OBB.GetScale().y, OBB.GetRotation(), Center, Radius))
		return false;

	glm::mat3x3 inverseMatrix;
	glm::mat3x3 matrix;

	/*JOKINERROR: CHECK THIS*/
	inverseMatrix = glm::translate(matrix, glm::vec2(-OBB.GetPos()));
	inverseMatrix = glm::rotate(matrix, -OBB.GetRotation());


	matrix = glm::translate(matrix, glm::vec2(OBB.GetPos()));
	matrix = glm::rotate(matrix, OBB.GetRotation());

	(void)pResult;
	/*glm::vec3 temp=
	StaticRectToStaticCircleEx( &(inverseMatrix * Center), OBB.GetScale().x, OBB.GetScale().y,  glm::vec2(0,0), Radius, pResult);

	//HACK! : Everything was on the opposite side of the circle, so I just rotated it by PI
	auto temp = (Matrix33::Translate(Center.x, Center.y) * Matrix33::RotRad(OBB.GetRotation()) * Matrix33::RotRad(PI));

	pResult->mNormal = temp * pResult->mNormal;
	pResult->mNormal = glm::normalize(pResult->mNormal);
	pResult->mPi =  temp * (pResult->mPi);
	*/
	return true;

}

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
bool StaticRectToStaticRectEx(const glm::vec2 &pos1, const glm::vec2 &size1, const glm::vec2 &pos2, const glm::vec2 &size2, Contact * pResult)
{
	if (pResult == nullptr)
		return StaticRectToStaticRect(pos1, size1.x, size1.y, pos2, size2.x, size2.y);

	if (!StaticRectToStaticRect(pos1, size1.x, size1.y, pos2, size2.x, size2.y))
		return false;

	glm::vec2 sum = size1 + size2;
	//get the vector
	glm::vec2 result = pos2 - pos1;

	//compute penetrations

	//compute penetrations
	float xPen = sum.x / 2 - glm::length(glm::proj(result, glm::vec2(1, 0)));
	float yPen = sum.y / 2 - glm::length(glm::proj(result, glm::vec2(0, 1)));

	//depending on the penetration set the normal and the pen.
	if (xPen >= yPen)
	{
		pResult->mNormal = glm::vec2(0, 1);
		pResult->mPenetration = yPen;
	}
	else
	{
		pResult->mNormal = glm::vec2(1, 0);
		pResult->mPenetration = xPen;
	}

	//pResult->mPenetration = min(xPen, yPen);

	//check if we should inverse the penetration
	if (glm::dot(result, pResult->mNormal) < 0)
		pResult->mNormal = -pResult->mNormal;

	//save the points of the box
	glm::vec2 vectors[4];
	vectors[0] = glm::vec2(pos2.x - size2.x / 2, pos2.y + size2.y / 2);
	vectors[1] = glm::vec2(pos2.x + size2.x / 2, pos2.y + size2.y / 2);
	vectors[2] = glm::vec2(pos2.x - size2.x / 2, pos2.y - size2.y / 2);
	vectors[3] = glm::vec2(pos2.x + size2.x / 2, pos2.y - size2.y / 2);

	//return the first vector contained in the rect as the point of intersection
	for (glm::vec2 vec : vectors)
	{
		if (StaticPointToStaticRect(vec, pos1, size1.x, size1.y))
		{
			pResult->mPi = vec;
			return true;
		}
	}
	return true;
}

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
bool OrientedRectToOrientedRectEx(Transform2D & OBB1, Transform2D & OBB2, Contact * pResult)
{
	//if (pResult == nullptr)
	//{
	//	return StaticPointToOrientedRect(&OBB1->mPosition, &OBB2->mPosition, OBB2->mScale.x, OBB2->mScale.y, OBB2->mOrientation);
	//}

	/*
	OBB1.SetAngle(glm::degrees(0.628318608));
	OBB2.SetAngle(glm::degrees(-0.453785539));

	OBB1.SetScale(glm::vec2(200, 160));
	OBB2.SetScale(glm::vec2(100, 100));

	OBB1.SetPos(glm::vec2(-19, -3));
	OBB2.SetPos(glm::vec2(98, -56));
	*/

	glm::mat3x3 rotA = glm::rotate(glm::mat3x3(), OBB1.GetRotation());
	glm::mat3x3 rotB = glm::rotate(glm::mat3x3(), OBB2.GetRotation());

	//OBB1
	glm::vec2 axisAx(rotA * glm::vec3(glm::vec2(abs(OBB1.GetScale().x) / 2, 0), 0));
	glm::vec2 axisAy(rotA * glm::vec3(glm::vec2(0, abs(OBB1.GetScale().y) / 2), 0));

	//OBB2
	glm::vec2 axisBx(rotB * glm::vec3(glm::vec2(abs(OBB2.GetScale().x) / 2, 0), 0));
	glm::vec2 axisBy(rotB * glm::vec3(glm::vec2(0, abs(OBB2.GetScale().y) / 2), 0));

	glm::vec2 distance = (OBB2.GetPos() - OBB1.GetPos());


	float pen1 = glm::length(axisAx) + glm::length(glm::proj(axisBx, axisAx)) + glm::length(glm::proj(axisBy, axisAx)) - glm::length(glm::proj(distance, axisAx));
	float pen2 = glm::length(axisAy) + glm::length(glm::proj(axisBx, axisAy)) + glm::length(glm::proj(axisBy, axisAy)) - glm::length(glm::proj(distance, axisAy));
	float pen3 = glm::length(axisBx) + glm::length(glm::proj(axisAx, axisBx)) + glm::length(glm::proj(axisAy, axisBx)) - glm::length(glm::proj(distance, axisBx));
	float pen4 = glm::length(axisBy) + glm::length(glm::proj(axisAx, axisBy)) + glm::length(glm::proj(axisAy, axisBy)) - glm::length(glm::proj(distance, axisBy));

	float minPen = glm::min(glm::min(glm::min(pen1, pen2), pen3), pen4);

	//then it doesnt collide
	if (minPen < 0)
		return false;

	//it collides
	if (pResult == nullptr)
		return true;

	pResult->mPenetration = minPen;

	glm::vec2 minAxis;

	if (minPen == pen1)
	{
		minAxis = axisAx;
		axisAx = glm::normalize(axisAx);
		pResult->mNormal = axisAx;
	}
	if (minPen == pen2)
	{
		minAxis = axisAy;
		axisAy = glm::normalize(axisAy);
		pResult->mNormal = axisAy;
	}
	if (minPen == pen3)
	{
		minAxis = axisBx;
		axisBx = glm::normalize(axisBx);
		pResult->mNormal = axisBx;
	}
	if (minPen == pen4)
	{
		minAxis = axisBy;
		axisBy = glm::normalize(axisBy);
		pResult->mNormal = axisBy;
	}

	if (glm::dot(pResult->mNormal, distance) < 0)
		pResult->mNormal = -pResult->mNormal;

	std::array<glm::vec2, 4> vertexA;
	std::array<glm::vec2, 4> vertexB;

	//we compute the vertices

	vertexA[0] = glm::vec2(rotA * glm::vec3(glm::vec2(-abs(OBB1.GetScale().x) / 2, +abs(OBB1.GetScale().y) / 2), 0)) + OBB1.GetPos();
	vertexA[1] = glm::vec2(rotA * glm::vec3(glm::vec2(-abs(OBB1.GetScale().x) / 2, -abs(OBB1.GetScale().y) / 2), 0)) + OBB1.GetPos();
	vertexA[2] = glm::vec2(rotA * glm::vec3(glm::vec2(+abs(OBB1.GetScale().x) / 2, -abs(OBB1.GetScale().y) / 2), 0)) + OBB1.GetPos();
	vertexA[3] = glm::vec2(rotA * glm::vec3(glm::vec2(+abs(OBB1.GetScale().x) / 2, +abs(OBB1.GetScale().y) / 2), 0)) + OBB1.GetPos();
													 								
	vertexB[0] = glm::vec2(rotB * glm::vec3(glm::vec2(-abs(OBB2.GetScale().x) / 2, +abs(OBB2.GetScale().y) / 2), 0)) + OBB2.GetPos();
	vertexB[1] = glm::vec2(rotB * glm::vec3(glm::vec2(-abs(OBB2.GetScale().x) / 2, -abs(OBB2.GetScale().y) / 2), 0)) + OBB2.GetPos();
	vertexB[2] = glm::vec2(rotB * glm::vec3(glm::vec2(+abs(OBB2.GetScale().x) / 2, -abs(OBB2.GetScale().y) / 2), 0)) + OBB2.GetPos();
	vertexB[3] = glm::vec2(rotB * glm::vec3(glm::vec2(+abs(OBB2.GetScale().x) / 2, +abs(OBB2.GetScale().y) / 2), 0)) + OBB2.GetPos();

	//check if its in the obb1
	if ((minAxis.x == axisAx.x  && minAxis.y == axisAx.y) || (minAxis.x == axisAy.x && minAxis.y == axisAy.y))
	{
		for (unsigned i = 0; i < vertexA.size(); i++)
		{
			if (StaticPointToOrientedRect(vertexA[i], OBB2.GetPos(), abs(OBB2.GetScale().x), abs(OBB2.GetScale().y), OBB2.GetRotation()))
			{
				pResult->mPi = vertexA[i];
				return true;
			}
		}


	}
	else
	{
		for (unsigned i = 0; i < vertexB.size(); i++)
		{
			if (StaticPointToOrientedRect(vertexB[i], OBB1.GetPos(), abs(OBB1.GetScale().x), abs(OBB1.GetScale().y), OBB1.GetRotation()))
			{
				pResult->mPi = vertexB[i];
				return true;
			}
		}
	}
	return true;
}
