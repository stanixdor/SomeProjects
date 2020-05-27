/*!
******************************************************************************
\file
\author  Joaquin Bilbao
\par     DP email: joaquin.bilbao\@digipen.edu
\par     GAME200
\par     milestone
\date    19/10/2017

\brief
this file contains the implementation for the collisionTable.h

Hours spent on this assignment:
*******************************************************************************/
#include "Engine\Physics\CollisionTable.h"
#include "Platform/MessageWindow/MessageWindow.h"
#include <memory> //smart ptrs
#include <cctype> //std::tolower
#include <algorithm> //std::transform
#include <iostream>

//this are static variables and should have this.
u32 CollisionGroup::idGenerator = 1;
std::map<std::string, CollisionGroup> CollisionGroup::collisionGroups;


void CollisionTable::Initialize()
{
	CollisionGroup::CreateCollisionGroup("default");
	CollisionGroup::newCollisionId();
}

void CollisionTable::AddPolicy(u32 left, u32 right, CollisionPolicy policy)
{
	mData[left | right] = policy;
}

CollisionPolicy CollisionTable::GetCollisionPolicy(u32 left, u32 right)
{
	//look for the collision we want
	auto it = mData.find(left | right);
	//if we find it return the policy
	if (it != mData.end())
		return it->second;
	//otherwise we do the default behaviour (resolving it)
	return CollisionPolicy::Resolve;
}


//collision groups


CollisionGroup::CollisionGroup(std::string name) : mId(idGenerator), mName(name)
{
	newCollisionId();
}

CollisionGroup::CollisionGroup()
{
	//hardcoded AF
	mName = "default";
	mId = 1;
}

CollisionGroup::CollisionGroup(CollisionGroup & other)
{
	mId = other.getId();
	mName = other.mName;
}

CollisionGroup& CollisionGroup::CreateCollisionGroup(const std::string& GroupName)
{

	//all names are saved as lowercase
	std::string realName = GroupName;
	std::transform(GroupName.begin(), GroupName.end(), realName.begin(), std::tolower);

	//checks if the name already exists
	auto it = collisionGroups.find(realName);

	//if this name exists
	if (it != collisionGroups.end())
	{
		//create error message
		MessageWindow::errorMessage("Glitch in the matrix", "There is already a collision group of name " + GroupName);
		return GetCollisionGroup("Default");
	}
	else
	{
		//otherwise create the collision group
		//JOKINNEW
		collisionGroups[realName] = CollisionGroup(realName);

		return collisionGroups[realName];
	}
}


CollisionGroup& CollisionGroup::GetCollisionGroup(const std::string& GroupName)
{
	//all names are saved as lowercase
	std::string realName = GroupName;
	std::transform(GroupName.begin(), GroupName.end(), realName.begin(), std::tolower);

	//checks if the name already exists
	auto it = collisionGroups.find(realName);

	//if this name is not found
	if (it == collisionGroups.end())
	{
		//create error message
		MessageWindow::errorMessage("Glitch in the matrix", "Theres no collision group of name" + GroupName + " passing default collision group.");
		return GetCollisionGroup("default");
	}
	else
	{
		return collisionGroups[realName];
	}
}


u32 CollisionGroup::newCollisionId()
{
	u32 res = idGenerator;
	idGenerator <<= 1;
	return res;
}


CollisionGroup::operator u32() const
{
	return mId;
}

u32 CollisionGroup::getId()
{
	return mId;
}

CollisionGroup GetCollisionGroup(const std::string& GroupName)
{
	return CollisionGroup::GetCollisionGroup(GroupName);
}

bool CollisionGroup::operator==(CollisionGroup& other)
{
	return (mName == other.mName);
}

bool CollisionGroup::operator!=(CollisionGroup& other)
{
	return !(*this == other);
}


