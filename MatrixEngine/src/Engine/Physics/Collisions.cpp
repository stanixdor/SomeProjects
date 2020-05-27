// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		Collisions.cpp
//	Purpose:		Contains the functions to compute collisions
//	Project:		CS230_Joaquin.Bilbao_2
//	Author:			Joaquin Bilbao, Joaquin.Bilbao, 540002415
// ----------------------------------------------------------------------------
#include "Collisions.h"
#include "../GLM/gtx/matrix_transform_2d.hpp"

// ---------------------------------------------------------------------------

// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticPointToStaticCircle
// \brief	This function determines if a static point is inside a static circle
// 			- P:		The point to test for circle containment
// 			- Center:	Center of the circle
// 			- Radius:	Radius of the circle
// 
//  \return	true if the point is contained in the circle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToStaticCircle(const glm::vec2 &P, const glm::vec2 &Center, float Radius)
{
	float d = glm::length(P - Center);

	//if the distance between the two poins is lesser than the radius
	if (d <= Radius)
		return true;
	else
		return false;
}

// @TODO
//! ---------------------------------------------------------------------------
//	\fn		StaticPointToStaticRect
//	\brief	This function checks if the point Pos is colliding with the rectangle
//			whose center is Rect, width is "Width" and height is Height
//
//  \return	true if the point is contained in the rectangle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToStaticRect(const glm::vec2 &Pos, const glm::vec2 &Rect, float Width, float Height)
{
	//if this is true then the X of the point is between the X limits of the rectangle
	bool trueOnX = ((Rect.x - Width / 2) <= Pos.x) && (Pos.x <= (Rect.x + Width / 2));
	//if this is true then the Y of the point is between the Y limits of the rectangle
	bool trueOnY = ((Rect.y - Height / 2) <= Pos.y) && (Pos.y <= (Rect.y + Height / 2));

	//if both are true
	if ((trueOnX && trueOnY))
		return true;

	return false;
}

// @TODO
//! ---------------------------------------------------------------------------
//	\fn		StaticPointToOrientedRect
//	\brief	This function checks if the point Pos is colliding with an oriented rectangle
//			whose center is Rect, width is "Width", height is Height and rotation "AngleRad"
//
//  \return	true if the point is contained in the rectangle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticPointToOrientedRect(const glm::vec2 &Pos, const  glm::vec2 &Rect, float Width, float Height, float AngleRad)
{
	float degrees = glm::degrees(AngleRad);
	//Pos = inverse rotation * inverse Translation * pos
	glm::vec3 result;


	glm::mat3x3 matrix;

	/*create a matrix that holds the translation*/
	matrix = glm::translate(matrix, glm::vec2(-Rect.x, -Rect.y));

	/*Modify it to hold the rotation*/
	matrix = glm::rotate(matrix, degrees);

	/*apply the transformation to the vector*/
	result = matrix * glm::vec3(Pos, 0);

	/*return the result of point vs rect*/
	return StaticPointToStaticRect(glm::vec2(result), glm::vec2(0, 0), Width, Height);
}

// @TODO
//! ---------------------------------------------------------------------------
//	\fn		StaticCircleToStaticCircle
//	\brief	This function checks if the point Pos is colliding with an oriented rectangle
//			whose center is Rect, width is "Width", height is Height and rotation "AngleRad"
//
//  \return	true if the point is contained in the rectangle, false otherwise.
// ---------------------------------------------------------------------------
bool StaticCircleToStaticCircle(const glm::vec2 &Center0, float Radius0, const glm::vec2 &Center1, float Radius1)
{
	return StaticPointToStaticCircle(Center0, Center1, (Radius0 + Radius1));
}

// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticRect
// \brief	This functions checks if 2 rectangles are colliding
//			Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
//			Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
//
//  \return	true if both rectangles overlap, false otherwise.
// ---------------------------------------------------------------------------
bool StaticRectToStaticRect(const glm::vec2 &Rect0, float Width0, float Height0, const  glm::vec2 &Rect1, float Width1, float Height1)
{
	return StaticPointToStaticRect(Rect1, Rect0, (Width0 + Width1), (Height0 + Height1));
}

// @TODO
//! ---------------------------------------------------------------------------
// \fn		StaticRectToStaticCirlce
// \brief	This function checks if a circle is colliding with a rectangle
//			Rectangle: Defined at center "Rect" of width and height "Width", "Height", respectively
//			Center: Defined at center "Center", and of radius "Radius"
//
//  \return	true if both shapes overlap, false otherwise.
// ---------------------------------------------------------------------------
bool StaticRectToStaticCirlce(const glm::vec2 & Rect, float Width, float Height, const glm::vec2& Center, float Radius)
{
	//find nearest X and Y 
	float nearestX = glm::clamp(Center.x, Rect.x - Width / 2, Rect.x + Width / 2);
	float nearestY = glm::clamp(Center.y, Rect.y - Height / 2, Rect.y + Height / 2);

	//save a temp
	glm::vec2 point(nearestX, nearestY);

	return StaticPointToStaticCircle(point, Center, Radius);
}

// @TODO
//! ---------------------------------------------------------------------------
// \fn		OrientedRectToStaticCirlce
// \brief	This function checks if a circle is colliding with an oriented rectangle
//			Rectangle: Defined at center "Rect" of width and height "Width", "Height", 
//			and rotation "AngelRad" respectively
//			Center: Defined at center "Center", and of radius "Radius"
//
//  \return	true if both shapes overlap, false otherwise.
// ---------------------------------------------------------------------------
bool OrientedRectToStaticCirlce(const glm::vec2 & Rect, float Width, float Height, float AngleRad, const glm::vec2& Center, float Radius)
{
	float degrees = glm::degrees(AngleRad);
	//Pos = inverse rotation * inverse Translation * pos
	glm::vec3 result;

	glm::mat3x3 matrix;

	/*create a matrix that holds the translation*/
	matrix = glm::translate(matrix, glm::vec2(-Rect.x, -Rect.y));

	/*Modify it to hold the rotation*/
	matrix = glm::rotate(matrix, degrees);

	/*apply the transformation to the vector*/
	result = matrix * glm::vec3(Center, 0);

	/*return the result of rect vs circle*/
	return StaticRectToStaticCirlce(glm::vec2(result), Width, Height, glm::vec2(0, 0), Radius);

}
