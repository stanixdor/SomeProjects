/*****************************************************************/
/*Author: Joaquin Bilbao				                         */
/*Brief description: Implementation to get controller input	     */
/*****************************************************************/

#include "xboxController.h"

#include <Windows.h>
#include <XInput.h>
#include <iostream>

//give value to the static variable
std::array<bool, 4> xboxController::controllerConnected{};
//was extern, so we declare it here
std::array<xboxController, 4> controllerArray;
unsigned xboxController::controllerCount = 0;

/*Author: Joaquin Bilbao (even though Mikel Torrecilla helped me with this)
Description: Gets the input of every controller*/
void controllerInput()
{
	DWORD dwResult;

	//for the 4 max controllers
	for (DWORD i = 0; i < xboxController::controllerCount; i++)
	{
		XINPUT_STATE state;

		ZeroMemory(&state, sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &state);

		//Check if the controller is conected
		if (dwResult == ERROR_SUCCESS)
		{
			xboxController::controllerConnected[i] = true;
		}
		else
		{
			xboxController::controllerConnected[i] = false;
			return;
		}


		//save the input of the last frame
		controllerArray[i].previousButtonArray = controllerArray[i].buttonArray;

		//Get the controller joystick value, used for angles
		controllerArray[i].rightX = (float)state.Gamepad.sThumbRX;
		controllerArray[i].rightY = (float)state.Gamepad.sThumbRY;
		controllerArray[i].leftX = (float)state.Gamepad.sThumbLX;
		controllerArray[i].leftY = (float)state.Gamepad.sThumbLY;

		//glm::Clamp the movement between 0 and 1, used for movement
		controllerArray[i].real_rightY = controllerArray[i].rightY / 32768.0f;
		controllerArray[i].real_rightX = controllerArray[i].rightX / 32768.0f;
		controllerArray[i].real_leftX =  controllerArray[i].leftX / 32768.0f;
		controllerArray[i].real_leftY =  controllerArray[i].leftY / 32768.0f;

		controllerArray[i].rightX /= 32768.0f;
		controllerArray[i].rightY /= 32768.0f;
		controllerArray[i].leftX /= 32768.0f;
		controllerArray[i].leftY /= 32768.0f;

		//Make it less sensible so naive movements are ignored
		if (controllerArray[i].rightX < 0.5f && controllerArray[i].rightX > -0.5f)
			controllerArray[i].rightX = 0.0f;
		if (controllerArray[i].rightY < 0.5f && controllerArray[i].rightY > -0.5f)
			controllerArray[i].rightY = 0.0f;
		if (controllerArray[i].leftX < 0.5f && controllerArray[i].leftX > -0.5f)
			controllerArray[i].leftX = 0.0f;
		if (controllerArray[i].leftY < 0.5f && controllerArray[i].leftY > -0.5f)
			controllerArray[i].leftY = 0.0f;

		//Get if the LT and RT are pressed
		controllerArray[i].Rtrigger = state.Gamepad.bRightTrigger;
		controllerArray[i].Ltrigger = state.Gamepad.bLeftTrigger;

		//Manage the buttons
		for (unsigned j = 0; j < 16; ++j)
		{
			short shift = (1 << j);
			controllerArray[i].buttonArray[j] = (state.Gamepad.wButtons & shift);

			//Set the button to 0 or 1

			controllerArray[i].buttonArray[j] /= (unsigned short)pow(2, j);
		}


		controllerArray[i].ID = i;
	}
}

/*Author: Joaquin Bilbao
Description: gets the number of controllers connected and sets the bool array to true or false
if they are connected or not*/
int getControllers()
{
	DWORD dwResult;

	int controllers = 0;

	for (DWORD i = 0; i < 4; i++)
	{
		XINPUT_STATE state;

		ZeroMemory(&state, sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &state);

		//Check if the controller is conected
		if (dwResult == ERROR_SUCCESS)
		{
			xboxController::controllerConnected[i] = true;
			controllers++;
		}
		else
		{
			xboxController::controllerConnected[i] = false;
		}
	}
	xboxController::controllerCount = controllers;
	return controllers;
}


/*Author: Joaquin Bilbao
Description: returns true if the button has been pressed and false if not*/
bool xboxController::buttonIsPressed(unsigned short button)
{
	if (buttonArray[button] == 0)
		return false;
	return true;
}


/*Author: Joaquin Bilbao
Description: returns true the first time the button is pressed, if the last frame had this button pressed, then we return false*/
bool xboxController::buttonIsTriggered(unsigned short button)
{
	//if the button has been pressed this frame
	if (buttonArray[button])
		//if the last frame the button was not pressed
		if (previousButtonArray[button] == false)
			return true;

	//otherwise we return false
	return false;
}

/*Author: Joaquin Bilbao
Description: vibrates the controller*/ 
void xboxController::vibrate(float right, float left)
{
	if (left > 1.0f)
	{
		left = 1.0f;
	}
	if (right > 1.0f)
	{
		right = 1.0f;
	}

	XINPUT_VIBRATION vibrating;

	ZeroMemory(&vibrating, sizeof(XINPUT_VIBRATION));

	int leftmotor = int(left * 65535.0f);
	int rightmotor = int(left * 65535.0f);

	vibrating.wLeftMotorSpeed = (WORD)leftmotor;
	vibrating.wRightMotorSpeed = (WORD)rightmotor;
	
	XInputSetState(ID, &vibrating);

}

/*Author: Joaquin Bilbao
Description: stops controller vibration*/ 
void xboxController::stopvibrating()
{
	XINPUT_VIBRATION vibrating;

	int leftmotor = 0;
	int rightmotor = 0;

	vibrating.wLeftMotorSpeed = (WORD)leftmotor;
	vibrating.wRightMotorSpeed = (WORD)rightmotor;

	XInputSetState(ID, &vibrating);
}

/*Author: Joaquin Bilbao
Description: returns a free xboxcontroller*/ 
xboxController * getFreeController()
{
	for (auto & controller : controllerArray)
	{
		if (controller.used == false)
		{
			controller.used = true;
			return &controller;
		}
	}

	return nullptr;
}

/*Author: Joaquin Bilbao
Description: checks if anyone pressed a button*/ 
bool anyonePressedButton(unsigned short button)
{
	for (auto controller : controllerArray)
	{
		if (controller.buttonIsTriggered(button))
			return true;
	}

	return false;
}