#pragma once

#include <windows.h>
#include <Xinput.h>
#include "XButtonIDs.h"
#include "InputDevice.h"

class Gamepad : public InputDevice
{
public:

	Gamepad();

	// Constructor specifying which connected Gamepad to use
	Gamepad(int gamepadIndex);

	~Gamepad();

	// Get the current Gamepad state
	XINPUT_STATE GetCurrentGamepadState();
	
	// Get the index of the Gamepad connected [1-4]
	int GetGamepadIndex();

	// Is the Gamepad connected
	bool IsConnected();

	// Update Gamepad State, has to be called each frame
	void UpdateState();

	// Return true if the left/right Joystick is inside deadzone, false if outside
	bool IsLeftJoystickInDeadZone();
	bool IsRightJoystickInDeadZone();

	// Return the joystick axis value
	float GetLeftJoystickXValue();
	float GetLeftJoystickYValue();
	float GetRightJoystickXValue();
	float GetRightJoystickYValue();

	//Get left trigger value (gachette)
	float GetLeftTriggerValue();

	//Get right trigger value (gachette)
	float GetRightTriggerValue();

	// Vibrate the gamepad. 
	// 0.0f will cancel the current vibration, 1.0f will set max speed
	// The vibration will keep going until it is stopped calling the same function with 0.0f
	void Vibrate(float leftMotorIntensity = 0.0f, float rightMotorIntensity = 0.0f);

	// Is the button corresponding to buttonId pressed or not
	bool IsButtonPressed(int buttonId);

	/*
	For using the function follow:
	MyGamepad.UpdateState(); // Update the gamepad

	if (MyGamepad.IsButtonPressed(XButtons.A))
	// Jump...

	if (MyGamepad.IsButtonDownOnCurrentFrame(XButtons.X))
	// Punch...

	if (MyGamepad.IsButtonDownOnCurrentFrame(XButtons.Y))
	// Open inventory...

	// Update the gamepad for next frame
	MyGamepad.RecordLastState();
	*/
	// Has the button corresponding to buttonId been pressed on the current frame and not on the current on
	bool IsButtonDownOnCurrentFrame(int buttonId);
	
	// record last button states for next frame
	void RecordLastState();

	bool IsSelected(PlayerAction playerAction);

private:

	// Current gamepad state
	XINPUT_STATE m_currentGamepadState;
	
	// Gamepad index (eg. 1,2,3,4)
	int m_gamepadIndex;

	// Total gamepad buttons
	static const int GamepadButtonCount = 14; 

	// Previous frame button states
	bool m_previousGamepadButtonPressed[GamepadButtonCount];

	// Current frame button states
	bool m_currentGamepadButtonPressed[GamepadButtonCount];

	// Buttons pressed on current frame
	bool m_gamepadButtonDownOnCurrentFrame[GamepadButtonCount];

	const float SHORT_TO_FLOAT_CONVERSION_FACTOR = 32768.0f;
	const float BYTE_NORMALIZATION_FACTOR = 255.0f;
	const float NORMALIZATION_TO_UNSIGNED_INTEGER_CONVERSION_FACTOR = 65535.0f;
};

