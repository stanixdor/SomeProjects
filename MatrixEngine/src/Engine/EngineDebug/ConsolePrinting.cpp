#include "ConsolePrinting.h"
#include <iostream>

std::stringstream Console::textBuffer;

void Console::Write(const std::stringstream& string)
{
	textBuffer << string.str();
}

void Console::WriteLine(const std::stringstream& string)
{
	textBuffer << string.str() << std::string("\n");
}


