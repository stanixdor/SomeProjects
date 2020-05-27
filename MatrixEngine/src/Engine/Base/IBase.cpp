#include "IBase.h"

IBase::~IBase() {}

//const RTTI & IBase::GetType() const
//{
//	return type;
//}
//
//
//const RTTI IBase::type(typeid(IBase).name(), static_cast<const IBase *>(nullptr));

std::map<std::string, std::unique_ptr<ICreator>> Creators;