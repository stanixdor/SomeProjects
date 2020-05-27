#pragma once
#include <iostream>
#include <ostream>

struct Color
{
	Color() = default;
	Color(unsigned rr, unsigned gg, unsigned bb);
	Color(const Color& c) = default;
	Color(Color&& c) = default;
	Color& operator = (const Color& c) = default;
	Color& operator = (Color&& c) = default;
	friend bool operator == (const Color& c1, const Color& c2);
	friend std::ostream& operator << (std::ostream& out, const Color& c);

	unsigned r = 0, g = 0, b = 0;
};
