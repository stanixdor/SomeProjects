/*****************************************************************/
/*Author: Joaquin Bilbao				                         */
/*Brief description: 											 */
/*****************************************************************/ 


/* This part provides a custom module that allows to uniquely identify the type of an object.

Its underlying mechanism relies on the standard typeid operator.

This will be later required to classify events and subscribers by type. */
#pragma once
#include <typeinfo> //typeid
#include <string> //strings

class TypeInfo
{
public:
	//constructor recieves all types
	/*!
	*****************************************************************************
	\fn
		template <typename T> 
		TypeInfo(const T& input): mName(std::string(typeid(input).name()))
	\brief
		constructor for TypeInfo, saves the typeid of the input as an std::string
	\param
		const T& input: input of the type to be saved in this class
	\return
		none
	*******************************************************************************/ 
	template <typename T> 
	TypeInfo(const T& input): mName(std::string(typeid(input).name()))
	{
	}
	
	/*!
	*****************************************************************************
	\fn
		TypeInfo(const std::type_info& input): mName(std::string((input).name()))
	\brief
		constructor for TypeInfo, saves the input entered as a typeid as an std::string
	\param
		const std::type_info& input: the type info of the object
	\return
		none
	*******************************************************************************/ 
	TypeInfo(const std::type_info& input): mName(std::string((input).name()))
	{
	}
	
	
	bool operator==(const TypeInfo& rhs) const;
	bool operator!=(const TypeInfo& rhs) const;
	bool operator<(const TypeInfo& rhs) const;
	bool operator>(const TypeInfo& rhs) const;
	
	const std::string& get_name() const;
	
private:
	//save name of the type
	std::string mName;
	
};

/*!
*****************************************************************************
\fn
	template <typename T> 
	inline const TypeInfo type_of(const T& input)
\brief
	returns a typeinfo class of type T
\param
	const T& input: an instance of the type we want to make a typeinfo
\return
	returns a typeinfo class of type T
*******************************************************************************/ 
template <typename T> 
const TypeInfo type_of(const T& input)
{
	return TypeInfo(input);
}

/*!
*****************************************************************************
\fn
	template <typename T> 
	inline const TypeInfo type_of()
\brief
	returns a typeinfo class of type T
\param
	none	
\return
	returns a typeinfo class of type T
*******************************************************************************/ 
template <typename T> 
const TypeInfo type_of()
{
	//this should be replaced by a having a type in every class
	return TypeInfo(typeid(T));
}
