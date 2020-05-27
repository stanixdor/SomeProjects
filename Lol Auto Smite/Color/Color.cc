#include "Color.hh"

Color::Color(unsigned rr, unsigned gg, unsigned bb) : r(rr), g(gg), b(bb)
{
}

bool operator==(const Color& c1, const Color& c2)
{
	if (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& out, const Color& c)
{
	out << "R: " << c.r << " G: " << c.g << " B " << c.b << "\n";
	return out;
}
