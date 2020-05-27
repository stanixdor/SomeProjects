#pragma once
#include "Engine\Platform\EventSystem\event.h"
#include "Platform\Input\Input.h"

class MouseClickEvent : public Messaging::Event
{
public:
	MouseClickEvent(const glm::vec2& mousePosition);
	
	glm::vec2 mousePos;
};

class MouseReleaseEvent : public Messaging::Event
{
public:
	MouseReleaseEvent(const glm::vec2& mousePosition);

	glm::vec2 mousePos;
};