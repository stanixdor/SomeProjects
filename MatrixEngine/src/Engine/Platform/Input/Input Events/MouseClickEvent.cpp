#include "MouseClickEvent.h"

MouseClickEvent::MouseClickEvent(const glm::vec2 & mousePosition): mousePos(mousePosition)
{
}

MouseReleaseEvent::MouseReleaseEvent(const glm::vec2 & mousePosition) : mousePos(mousePosition)
{
}
