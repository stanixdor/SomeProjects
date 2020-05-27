#pragma once
#include <vector>
#include <string>
#include<sstream>

class Console
{
public:
	static void Write(const std::stringstream& string);
	static void WriteLine(const std::stringstream& string);
	static std::stringstream textBuffer;

};