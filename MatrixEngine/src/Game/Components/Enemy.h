#pragma once

#include "../../MatrixEngine.h"
#include <iostream>

class Enemy : public Component, public serialization::Serializable
{
public:
	SERIALIZABLE;
	void Start()
	{
		Json::Value val;
		to_json(val);
		std::cout << *this;
	}
	void Update()
	{

	}
	float f = 69.f;
	int i = 69;
	std::string s = "soy enemy";

};

REFLECTION(Enemy, f, i, s);