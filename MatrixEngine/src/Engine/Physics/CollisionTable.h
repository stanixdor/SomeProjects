/*!
******************************************************************************
\file    CollisionTable.h
\author  Joaquin Bilbao
\par     DP email: joaquin.bilbao\@digipen.edu
\par     GAME200
\par     segment prototype milestone
\date    30/10/2017

\brief
this file contains the implementation for the collision tables and collision groups

Hours spent on this assignment:
*******************************************************************************/
#pragma once
#include "Utils/ISingleton.h"
#include <map>

using u32 = unsigned int;

enum CollisionPolicy { Resolve, SkipEvents, SkipResolution, SkipDetection };

/*****************************************************************/
/*Author: Joaquin Bilbao                                         */
/*Brief description: This class contains the three possible ways */
/*of resolving a collision (EPolicy). When an ID is passed, the  */
/*correct resolution method is applied.                          */
/*****************************************************************/
class CollisionTable : public ISingleton<CollisionTable>
{
public:

	void Initialize();
	/*!
	*****************************************************************************
	\fn
	void AddPolicy(u32 left, u32 right, CollisionPolicy policy)

	\brief
	this function will add a collision resolution policy to a collision between two
	collision Groups
	\param
	u32 left: first collision group
	u32 right: second collision group

	CollisionPolicy policy: how to resolve the collision
	\return
	no return.
	*******************************************************************************/
	void AddPolicy(u32 left, u32 right, CollisionPolicy policy);
	/*!
	*****************************************************************************
	\fn
	CollisionPolicy CheckCollisionGroup(u32 left, u32 right)

	\brief
	checks the collision group and indicates how to resolve the collision
	\param
	u32 left: first collision group
	u32 right: second collision group
	\return returns the collision group policy
	*******************************************************************************/
	CollisionPolicy GetCollisionPolicy(u32 left, u32 right);

private:
	std::map<u32, CollisionPolicy> mData; // <combined bit code, resolve policy>

};

/*****************************************************************/
/*Author: Joaquin Bilbao                                         */
/*Brief description: Every collision group will have its own ID  */
/*Then we associate a name to a instance of a collision group to */
/*make it user friendly                                          */
/*****************************************************************/
class CollisionGroup
{
public:

	/*!
	*****************************************************************************
	\fn
	CollisionGroup::CollisionGroup() : mId(idGenerator)

	\brief
	constructor for the collision group. after setting the ID we advance to the
	next ID.
	\param
	none
	\return
	none
	*******************************************************************************/
	CollisionGroup(std::string name);

	CollisionGroup();
	static u32 idGenerator;

	/*!
	*****************************************************************************
	\fn
	CollisionGroup* CreateCollisionGroup(const std::string& GroupName)

	\brief
	creates a collision Group and returns a ptr to that collision group
	\param
	const std::string& GroupName: Creates a collision group with that name
	\return
	returns a ptr to the CollisionGroup just created
	*******************************************************************************/
	static CollisionGroup& CreateCollisionGroup(const std::string& GroupName);

	/*!
	*****************************************************************************
	\fn
	CollisionGroup* GetCollisionGroup(const std::string& GroupName)

	\brief
	Returns a pointer to a collisionGroup of the passed name
	\param
	const std::string& GroupName: name of the collision group

	\return
	returns a ptr to the new collision group created.
	*******************************************************************************/
	static CollisionGroup& GetCollisionGroup(const std::string& GroupName);

	/*!
	*****************************************************************************
	\fn
	static u32 newCollisionId()
	\brief
	Creates a new ID for the current collision Group
	\param
	none
	\return
	returns the ID of the new collision group.
	*******************************************************************************/
	static u32 newCollisionId();

	/*!
	*****************************************************************************
	\fn
	explicit operator u32() const

	\brief
	Conversion constructor from CollisionGroup to u32
	\param
	Takes a *this implicitly.
	\return
	returns the ID of the collision group.
	\example
	u32 CollisionGroupID = static_cast<u32>(CollisionGroupExample);
	u32 CollisionGroupID = (u32)(CollisionGroupExample);
	*******************************************************************************/
	explicit operator u32() const;

	u32 getId();


	bool operator==(CollisionGroup& other);

	bool operator!=(CollisionGroup& other);
	CollisionGroup(CollisionGroup& other);


	u32 mId; // binary code. 
	std::string mName; // name of the collision group.
public:

	//save all created collision groups here
	static std::map<std::string, CollisionGroup> collisionGroups;
};


/*!
*****************************************************************************
\fn
CollisionGroup* GetCollisionGroup(const std::string& GroupName)

\brief
Wrapper for CollisionGroup::GetCollisionGroup
\param
const std::string& GroupName: name of the collision group

\return
returns a ptr to the new collision group created.
*******************************************************************************/
CollisionGroup GetCollisionGroup(const std::string& GroupName);
