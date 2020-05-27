#pragma once

#include <Windows.h>
#include <iostream>

struct Position
{
	Position() = default;
	Position(int xx, int yy);
	Position(const Position& p) = default;
	Position(Position&& p) = default;
	Position(const POINT& p);;
	Position(POINT&& p);
	Position& operator = (const Position& p) = default;
	Position& operator = (Position&& p) = default;
	Position& operator = (const POINT& p);
	Position& operator = (POINT&& p);
	int x = 0, y = 0;
	friend bool operator == (const Position& p1, const Position& p2);
	friend std::ostream& operator << (std::ostream& out, const Position& p);
};

class MouseUtilities
{
public:
	static void mouseLeftClick()
	{
		INPUT Input = { 0 };

		// left down
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));

		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	// click the mouse using "SendInput"
	static void mouseRightClick()
	{
		// create an INPUT object named "Input"
		INPUT Input = { 0 };

		// simulate the right mouse button being pressed
		// specify the type of input as mouse
		Input.type = INPUT_MOUSE;
		// specify the action that was performed, "right down"
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		// send the input to the system
		::SendInput(1, &Input, sizeof(INPUT));

		// simulate release of the right mouse button
		// clear the the "Input" object rather than assign a more memory
		::ZeroMemory(&Input, sizeof(INPUT));
		// specify type of input
		Input.type = INPUT_MOUSE;
		// indicate that the action "right up" was performed
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		// send the input to the system
		::SendInput(1, &Input, sizeof(INPUT));
	}

	// finds out information about the user's current screen configuration
	// then moves the cursor to the requested coordinates
	static void mouseMoveTo(int toX, int toY)
	{
		// get system information regarding screen size / resolution
		double screenRes_width = ::GetSystemMetrics(SM_CXSCREEN) - 1;
		double screenRes_height = ::GetSystemMetrics(SM_CYSCREEN) - 1;
		// scale the function inputs 'toX and 'toY' appropriately by a
		// factor of 65535
		double dx = toX * (65535.0f / screenRes_width);
		double dy = toY * (65535.0f / screenRes_height);
		// we now have variables 'dx' and 'dy' with the user-desired
		// coordinates in a form that the SendInput function can understand

		// set up INPUT Input, assign values, and move the cursor
		INPUT Input = { 0 };
		// we want to be sending a MOUSE-type input
		Input.type = INPUT_MOUSE;
		// the mouse input is of the 'MOVE ABSOLUTE' variety
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		Input.mi.dx = LONG(dx);
		Input.mi.dy = LONG(dy);
		// we have given information about the magnitude of the new
		// cursor position; here we will send it
		::SendInput(1, &Input, sizeof(INPUT));
	}
	static void mouseMoveTo(const Position& pos)
	{
		MouseUtilities::mouseMoveTo(pos.x, pos.y);
	}

	static void mouseScrollUp(DWORD factor = 1)
	{
		INPUT Input = { 0 };

		//scroll up
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = WHEEL_DELTA * factor;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	static void mouseScrollDown(DWORD factor = 1)
	{
		INPUT Input = { 0 };

		//scroll up
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = -WHEEL_DELTA * factor;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	static POINT GetMousePos()
	{
		POINT _cursor;
		GetCursorPos(&_cursor);
		return _cursor;
	}
};