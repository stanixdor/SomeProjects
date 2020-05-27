#include "InputManager.hh"
#include <Windows.h>

std::array<bool, 256> InputManager::keysPressedThisFrame;
std::array<bool, 256> InputManager::keysPressedPreviousFrame;

void InputManager::Initialize()
{
	for (auto& key : InputManager::keysPressedThisFrame)
		key = false;

	for (auto& key : InputManager::keysPressedPreviousFrame)
		key = false;
}

void InputManager::Update()
{
	int i = 0;
	for (auto& key : InputManager::keysPressedThisFrame)
	{
		if (::GetAsyncKeyState(i))
			key = true;
		else 
			key = false;
		++i;
	}
}

void InputManager::EndUpdate()
{
	for (int i = 0; i < 256; ++i)
		InputManager::keysPressedPreviousFrame[i] = InputManager::keysPressedThisFrame[i];
}

bool InputManager::IsKeyPressed(size_t key)
{
	if (InputManager::keysPressedThisFrame[key] && !InputManager::keysPressedPreviousFrame[key])
		return true;
	else 
		return false;
}

bool InputManager::IsKeyDown(size_t key)
{
	if (InputManager::keysPressedThisFrame[key])
		return true;
	else
		return false;
}

bool InputManager::IsKeyReleased(size_t key)
{
	if (!InputManager::keysPressedThisFrame[key] && InputManager::keysPressedPreviousFrame[key])
		return true;
	else
		return false;
}
