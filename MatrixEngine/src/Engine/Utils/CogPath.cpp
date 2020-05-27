// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivObject.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------


#include "CogPath.h"
#include "GameObject\GameObjectManager.h"
#include "Json\json.h"


GameObject * CogPath::FindObject()
{
	if (!target)
	{
		for (const auto & object : objectManager.m_gameObjects)
		{
			if (object->m_guid == id)
			{
				target = object.get();
				break;
			}
		}
	}

	return target;
}

void CogPath::set_id(size_t new_id)
{
	id = new_id;
	target = nullptr;
}

void CogPath::from_json(const Json::Value & val)
{
	id = val.asUInt64();
}

void CogPath::to_json(Json::Value & val) const
{
	val = id;
}

