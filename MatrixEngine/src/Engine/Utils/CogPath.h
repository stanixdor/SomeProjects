// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivObject.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once


class GameObject;

namespace Json
{
	class Value;
}

class CogPath
{
	public:
		void from_json(const Json::Value & val);
		void to_json(Json::Value & val) const;

		// link with the object with the cogpath and gind specific object id
		GameObject * FindObject();

		void set_id(size_t new_id);
		
	private:
		size_t id = size_t(-1);
		GameObject * target = nullptr;
};


