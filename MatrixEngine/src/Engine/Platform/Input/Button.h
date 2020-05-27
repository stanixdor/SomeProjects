#pragma once

#include "GameObject\Component.h"
#include "Platform\Input\Input Events\MouseClickEvent.h"

class Button : public Component
{
public: 

	SERIALIZABLE;
	Button();

	void Initialize();
	void Update();


	void buttonTriggered();
	void buttonHoveredAnimations();
	void resetButton();
	bool mouseHoveringThisButton();

	void onMouseClick(const MouseClickEvent& mouseClick);
	void onMouseRelease(const MouseReleaseEvent& mouseClick);
	void changeColor(const glm::vec4& newColor);

	std::string levelName;
	
	float expansionLimits;
	float expansionSpeed;
	float currentExpansion;

	glm::vec2 originalScale;

	glm::vec4 originalColor;
	glm::vec4 buttonHoveredColor;
	glm::vec4 buttonClickedColor;

	bool readyToActivate;
};