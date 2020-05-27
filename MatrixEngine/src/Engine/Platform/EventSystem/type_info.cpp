/*****************************************************************/
/*Author: Joaquin Bilbao				                         */
/*Brief description:	    									 */
/*****************************************************************/ 

#include "Engine/Platform/EventSystem/type_info.h"

/*!
*****************************************************************************
\fn const char* TypeInfo::get_name() const

\brief
	returns the name of the type as a const char*

\param
	none
\return
	the type id as a const char *
*******************************************************************************/ 
const std::string& TypeInfo::get_name() const
{
	return mName;
}

/*!
*****************************************************************************
\fn
	bool TypeInfo::operator==(const TypeInfo& rhs) const
\brief
	checks if two TypeInfo objects are of the same type
\param
	const TypeInfo& rhs: the other TypeInfo to be compared to
\return
	true if they are of the same type or false if they are not
*******************************************************************************/ 
bool TypeInfo::operator==(const TypeInfo& rhs) const
{
	return (get_name() == rhs.get_name());
}

/*!
*****************************************************************************
\fn
	bool TypeInfo::operator!=(const TypeInfo& rhs) const
\brief
	checks if two TypeInfo objects are of different types
\param
	const TypeInfo& rhs: the other TypeInfo to be compared to
\return
	true if they are of different types or false if they are of the same
*******************************************************************************/ 
bool TypeInfo::operator!=(const TypeInfo& rhs) const
{
	return !(*this == rhs);
}

bool TypeInfo::operator<(const TypeInfo& rhs) const
{
	return (get_name() < rhs.get_name());
}

bool TypeInfo::operator>(const TypeInfo& rhs) const
{
	return !(*this < rhs);
}