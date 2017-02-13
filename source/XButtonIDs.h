#pragma once

#include <windows.h>
#include <Xinput.h>

// XInput Button values
static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};

// XInput Button IDs
struct XButtonIDs
{
// Function prototypes
//---------------------//

XButtonIDs(); // Default constructor

// Member variables
//---------------------//

int A, B, X, Y; // 'Action' buttons

// Directional Pad (D-Pad)
int DPadUp, DPadDown, DPadLeft, DPadRight;

// Shoulder ('Bumper') buttons
int LeftShoulder, RightShoulder;

// Thumbstick buttons
int LeftThumbstick, RightThumbstick;

int Start; // 'START' button
int Back;  // 'BACK' button
};

// Externally define the 'XButtonIDs' struct as 'XButtons'
extern XButtonIDs XButtons;

