#pragma once

#include "../../MatrixEngine.h"
#include <iostream>

class Test : public Component
{
public:
	SERIALIZABLE;
	Test() { m_name = "Test"; }
	//RTTI_DECLARATION;
	void Initialize()
	{

	}
	void Update()
	{
		/*Json::Value val;
		to_json(val);
		std::cout << *this;*/
	}
	float f = 1.124f;
	int i = 34;
	std::string s = "soy penis";

};