/***********************************************************/
/*Author: Joaquin Bilbao				                   */
/*Brief description: contains function and structure       */
/* necessary to get the controller input                   */
/***********************************************************/


#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <iostream>
#include <array>

//enum for each button on the controller
enum Buttons
{
	padUp, padDown, padLeft, padRight, start, back, L3, R3, LB, RB, LT, RT, A, B, X, Y
};

struct xboxController
{
	xboxController() = default;

	bool buttonIsPressed(unsigned short button);
	bool buttonIsTriggered(unsigned short button);

	void vibrate(float right, float left);
	void stopvibrating();

	//saves left and right sticks input
	float leftX;
	float leftY;
	float rightX;
	float rightY;

	//same as above, but it will not be rounded
	float real_leftX;
	float real_leftY;
	float real_rightX;
	float real_rightY;

	//saves left and right triggers
	float Ltrigger;
	float Rtrigger;

	//array containing the buttons that have been pressed
	std::array<unsigned short, 16> buttonArray;
	std::array<unsigned short, 16> previousButtonArray;


	//ID of the controller
	unsigned ID;

	//The element on the array will have true if its connected, false otherwise
	static std::array<bool, 4> controllerConnected;
	//number of controllers 
	static unsigned controllerCount;
	bool used;
};

//array of xboxControllers
extern std::array<xboxController, 4> controllerArray;

xboxController* getFreeController();

bool anyonePressedButton(unsigned short button);
int getControllers();
void controllerInput();
#endif