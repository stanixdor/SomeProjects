#include "MouseUtilites.hh"
#include <utility>

Position::Position(int xx, int yy) : x(xx), y(yy)
{
}

Position::Position(const POINT& p)
{
	x = p.x;
	y = p.y;
}

Position::Position(POINT&& p)
{
	x = std::move(p.x);
	y = std::move(p.y);
}

Position& Position::operator=(const POINT& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

Position& Position::operator=(POINT&& p)
{
	x = std::move(p.x);
	y = std::move(p.y);
	return *this;
}

bool operator==(const Position& p1, const Position& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& out, const Position& p)
{
	out << "X: " << p.x << " Y: " << p.y << "\n";
	return out;
}
