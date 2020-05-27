#pragma once

#include "Engine/GameObject/Component.h"
#include "Utils\CogPath.h"

class TestPatherino : public Component
{
	SERIALIZABLE;
public:
	TestPatherino() { m_name = "TestPatherino"; }
	CogPath dummyPath;
};