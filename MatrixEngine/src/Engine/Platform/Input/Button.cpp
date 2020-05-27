#include "Button.h"
#include "Platform\EventSystem\event_dispatcher.h"
#include "Physics\Collisions.h"
#include "GameObject\GameObject.h"
#include "Engine\Logic\Level.h"
#include "Engine\Graphics\GraphicsComponents\Renderable.h"


REFLECTION(Button, m_name, levelName, originalScale, expansionLimits, expansionSpeed, originalColor, 
	buttonHoveredColor, buttonClickedColor);

Button::Button()
{
	m_name = "Button Component";
	currentExpansion = 0;
	expansionLimits = 0;
	expansionSpeed = 0;
}

void Button::Initialize()
{
	Messaging::EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Button::onMouseClick);
	temp->register_handler(this, &Button::onMouseRelease);


	originalScale = m_GameObject->m_transform->GetScale();
	Graphics::Renderable* render = m_GameObject->FindComponentByType<Graphics::Renderable>();

	render->setColor(originalColor);
	// subscribe this to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<MouseClickEvent>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<MouseReleaseEvent>());

}

void Button::Update()
{
	if (mouseHoveringThisButton())
		buttonHoveredAnimations();
	else
		resetButton();

}

void Button::buttonTriggered()
{
	ResetLevel(levelName);
}

void Button::buttonHoveredAnimations()
{

	if (abs(currentExpansion) < expansionLimits)
	{
		m_GameObject->m_transform->IncreaseScaleBy(glm::vec2(expansionSpeed, expansionSpeed));
		currentExpansion += expansionSpeed;
	}
	else
	{
		currentExpansion = 0;
		expansionSpeed = -expansionSpeed;
	}

	if (!readyToActivate)
		changeColor(buttonHoveredColor);
}

void Button::resetButton()
{
	//set the original scale
	m_GameObject->m_transform->SetScale(originalScale);

	//reset the expansion
	currentExpansion = 0;
	expansionSpeed = abs(expansionSpeed);

	changeColor(originalColor);
}

bool Button::mouseHoveringThisButton()
{
	return StaticPointToStaticRect(Input::Instance()->ScreenToWorld(), m_GameObject->m_transform->GetPos(), m_GameObject->m_transform->GetScale().x, m_GameObject->m_transform->GetScale().y);
}

void Button::onMouseClick(const MouseClickEvent & mouseClick)
{
	//if the mouse clicks on this button 
	if (StaticPointToStaticRect(mouseClick.mousePos, m_GameObject->m_transform->GetPos(), m_GameObject->m_transform->GetScale().x, m_GameObject->m_transform->GetScale().y))
	{
		readyToActivate = true;

		changeColor(buttonClickedColor);
	}
}

void Button::onMouseRelease(const MouseReleaseEvent & mouseClick)
{
	if (!readyToActivate)
		return;


	if (StaticPointToStaticRect(mouseClick.mousePos, m_GameObject->m_transform->GetPos(), m_GameObject->m_transform->GetScale().x, m_GameObject->m_transform->GetScale().y))
		buttonTriggered();

	else
		changeColor(originalColor);

	readyToActivate = false;
}

void Button::changeColor(const glm::vec4 & newColor)
{
	Graphics::Renderable* render = m_GameObject->FindComponentByType<Graphics::Renderable>();

	if (render != nullptr)
	{
		render->mColor = newColor;
	}
}
