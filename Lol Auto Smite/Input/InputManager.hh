#pragma once

#include <array>

class InputManager
{
public:
	static void Initialize();
	static void Update();
	static void EndUpdate();

	static bool IsKeyPressed(size_t key);
	static bool IsKeyDown(size_t key);
	static bool IsKeyReleased(size_t key);

private:
	static std::array<bool, 256> keysPressedThisFrame;
	static std::array<bool, 256> keysPressedPreviousFrame;
};