#include "Gamepad.h"

// Link the 'Xinput'
#pragma comment(lib,"Xinput9_1_0.lib")

Gamepad::Gamepad()
{
}

// Constructor specifying which connected Gamepad to use
Gamepad::Gamepad(int gamepadIndex)
{
	// Set gamepad index
	// XInput considers a range of [0-3] for up to 4 connected gamepads, we are using a range of [1-4] instead
	m_gamepadIndex = gamepadIndex - 1;

	// Initialize different gamepa dButton arrays
	for (int i = 0; i < GamepadButtonCount; i++)
	{
		m_previousGamepadButtonPressed[i] = false;
		m_currentGamepadButtonPressed[i] = false;
		m_gamepadButtonDownOnCurrentFrame[i] = false;
	}
}

// Get the current Gamepad state
XINPUT_STATE Gamepad::GetCurrentGamepadState()
{
	// Temporary XINPUT_STATE to return
	XINPUT_STATE GamepadState;

	// Zero memory
	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	XInputGetState(m_gamepadIndex, &GamepadState);

	// Return the gamepad state
	return GamepadState;
}

// Get the index of the Gamepad connected [1-4]
int Gamepad::GetGamepadIndex() const
{
	return m_gamepadIndex;
}

// Is the Gamepad connected
bool Gamepad::IsConnected()
{
	// Zero memory
	ZeroMemory(&m_currentGamepadState, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	DWORD Result = XInputGetState(m_gamepadIndex, &m_currentGamepadState);

	// Return true if the gamepad is connected
	return Result == ERROR_SUCCESS ? true : false;
}

// Update gamepad state
void Gamepad::UpdateState()
{
	// Obtain current gamepad state
	m_currentGamepadState = this->GetCurrentGamepadState(); 

	for (int i = 0; i < GamepadButtonCount; i++)
	{
		// Store which buttons are currently pressed or released based on the current gamepad state
		m_currentGamepadButtonPressed[i] = (m_currentGamepadState.Gamepad.wButtons & XINPUT_Buttons[i]) == XINPUT_Buttons[i];

		// Define if the button has been pressed on THIS frame
		m_gamepadButtonDownOnCurrentFrame[i] = !m_previousGamepadButtonPressed[i] && m_currentGamepadButtonPressed[i];
	}
}

// Update button states for next frame
void Gamepad::RecordLastState()
{
	memcpy(m_previousGamepadButtonPressed, m_currentGamepadButtonPressed, sizeof(m_previousGamepadButtonPressed));
}

// Deadzone check for Left Thumbstick
bool Gamepad::IsLeftJoystickInDeadZone()
{
	// Obtain the X & Y axes of the stick
	short joystickValueX = m_currentGamepadState.Gamepad.sThumbLX;
	short joystickValueY = m_currentGamepadState.Gamepad.sThumbLY;

	// X axis is outside of deadzone
	if (joystickValueX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		joystickValueX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return false;
	}

	// Y axis is outside of deadzone
	if (joystickValueY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		joystickValueY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return false;
	}

	// One (or both axes) axis is inside of deadzone
	return true;
}

// Deadzone check for Right Thumbstick
bool Gamepad::IsRightJoystickInDeadZone()
{
	// Obtain the X & Y axes of the stick
	short joystickValueX = m_currentGamepadState.Gamepad.sThumbRX;
	short joystickValueY = m_currentGamepadState.Gamepad.sThumbRY;

	// X axis is outside of deadzone
	if (joystickValueX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		joystickValueX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return false;
	}

	// Y axis is outside of deadzone
	if (joystickValueY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		joystickValueY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return false;
	}

	// One (or both axes) axis is inside of deadzone
	return true;
}

// Return X axis of left stick
float Gamepad::GetLeftJoystickXValue()
{
	//if (!this->IsLeftJoystickInDeadZone())
	// The left stick is outside of its deadzone
	//{
		// Use left stick input here...
	//}

	// Obtain X axis of left stick
	short joystickValueX = m_currentGamepadState.Gamepad.sThumbLX;

	// Return axis value, converted from short to float
	return (static_cast<float>(joystickValueX) / SHORT_TO_FLOAT_CONVERSION_FACTOR);
}

// Return Y axis of left stick
float Gamepad::GetLeftJoystickYValue()
{
	// Obtain Y axis of left stick
	short joystickValueY = m_currentGamepadState.Gamepad.sThumbLY;

	// Return axis value, converted from short to float
	return (static_cast<float>(joystickValueY) / SHORT_TO_FLOAT_CONVERSION_FACTOR);
}

// Return X axis of right stick
float Gamepad::GetRightJoystickXValue()
{
	// Obtain X axis of right stick
	short joystickValueX = m_currentGamepadState.Gamepad.sThumbRX;

	// Return axis value, converted from short to float
	return (static_cast<float>(joystickValueX) / SHORT_TO_FLOAT_CONVERSION_FACTOR);
}

// Return Y axis of right stick
float Gamepad::GetRightJoystickYValue()
{
	// Obtain the Y axis of the left stick
	short joystickValueY = m_currentGamepadState.Gamepad.sThumbRY;

	// Return axis value, converted from short to float
	return (static_cast<float>(joystickValueY) / SHORT_TO_FLOAT_CONVERSION_FACTOR);
}

//Get left trigger value
float Gamepad::GetLeftTriggerValue()
{
	// Obtain value of left trigger
	BYTE Trigger = m_currentGamepadState.Gamepad.bLeftTrigger;

	// If value superior to dead zone
	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		// Converting range of [0-255] to [0-1]
		return Trigger / BYTE_NORMALIZATION_FACTOR;
	}

	// Otherwise returns zero
	return 0.0f; 
}

//Get right trigger value
float Gamepad::GetRightTriggerValue()
{
	// Obtain value of right trigger
	BYTE Trigger = m_currentGamepadState.Gamepad.bRightTrigger;

	// If value superior to dead zone
	if (Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		// Converting range of [0-255] to [0-1]
		return Trigger / BYTE_NORMALIZATION_FACTOR;
	}

	// Otherwise returns zero
	return 0.0f; // Trigger was not pressed
}

// Vibrate the gamepad. 
// 0.0f will cancel the current vibration, 1.0f will set max speed
// The vibration will keep going until it is stopped calling the same function with 0.0f
void Gamepad::Vibrate(float leftMotorIntensity, float rightMotorIntensity)
{
	// Vibration state
	XINPUT_VIBRATION VibrationState;

	// Zero memory
	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	// Calculate vibration values
	// Convert [0-1] range to XInput’s default vibration values range from 0 to 65535
	int iLeftMotor = int(leftMotorIntensity * NORMALIZATION_TO_UNSIGNED_INTEGER_CONVERSION_FACTOR);
	int iRightMotor = int(rightMotorIntensity * NORMALIZATION_TO_UNSIGNED_INTEGER_CONVERSION_FACTOR);

	// Set vibration values
	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	// Set the vibration state
	XInputSetState(m_gamepadIndex, &VibrationState);
}

// Is the button corresponding to buttonId pressed or not
bool Gamepad::IsButtonPressed(int buttonId)
{
	// bitwise AND operation to see if buttonId is pressed
	return m_currentGamepadState.Gamepad.wButtons & XINPUT_Buttons[buttonId] ? true : false;
}

// Has the button corresponding to buttonId been pressed on the current frame and not on the current on
bool Gamepad::IsButtonDownOnCurrentFrame(int buttonId)
{
	return m_gamepadButtonDownOnCurrentFrame[buttonId];
}

bool Gamepad::IsSelected(PlayerAction playerAction)
{
	if (!this->IsConnected())
	{
		return false;
	}

	switch (playerAction)
	{
	case PlayerAction::MoveToTheLeft:
		return this->IsButtonPressed(XButtons.DPadLeft) || (!this->IsLeftJoystickInDeadZone() && this->GetLeftJoystickXValue() < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		break;

	case PlayerAction::MoveToTheRight:
		return this->IsButtonPressed(XButtons.DPadRight) || (!this->IsLeftJoystickInDeadZone() && this->GetLeftJoystickXValue() > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		break;

	case PlayerAction::Accelerate:
		return this->IsButtonPressed(XButtons.X); 
		break;

	case PlayerAction::GetDown:
		return this->IsButtonPressed(XButtons.DPadDown) || (!this->IsLeftJoystickInDeadZone() && this->GetLeftJoystickYValue() < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		break;

	case PlayerAction::Jump:
		return this->IsButtonPressed(XButtons.A);
		break;

	default:
		return false;
		break;
	}
}

Gamepad::~Gamepad()
{
}
