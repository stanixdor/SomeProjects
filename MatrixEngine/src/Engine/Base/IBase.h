#pragma once

#include <typeinfo>
#include <memory>
#include <map>
#include <string>
#include <type_traits>

//class RTTI
//{
//public:
//	template <typename T>
//	RTTI(const char* l_name, const T *);
//	const char* m_name;
//};

class IBase
{
public:
	virtual ~IBase() = 0;
	//virtual const RTTI& GetType() const;
	//
	//static const RTTI type;

};

class ICreator
{
public:
	virtual std::unique_ptr<IBase> Create() = 0;
};

template <class T>
class Creator : public ICreator
{
	virtual std::unique_ptr<IBase> Create() override
	{
		return std::make_unique<T>();
	}
};

template <>
class Creator<IBase> : public ICreator
{
	virtual std::unique_ptr<IBase> Create() override
	{
		return nullptr;
	}
};

extern std::map<std::string, std::unique_ptr<ICreator>> Creators;

template <typename T>
void RegisterType(const char * l_name)
{
	Creators.emplace(l_name, std::make_unique<Creator<T>>());
}

/*template <typename T>
::RTTI(const char* l_name, const T *) : m_name(l_name)
{
	//RegisterType<T>(l_name);
}*/

#define REGISTER_TYPE(TYPE) RegisterType<TYPE>(typeid(TYPE).name());

//#define RTTI_DECLARATION  \
//virtual const RTTI& GetType() const override {return type;}\
//static const RTTI type;\
//
//#define RTTI_IMPLEMENTATION(TYPE)   \
//const RTTI TYPE::type(typeid(TYPE).name(), static_cast<const TYPE *>(nullptr)); \
//static_assert(std::is_base_of<Component, TYPE>::value, "All components must derive from Component");

