#include "XButtonIDs.h"

// Define the 'XButtonIDs' struct as 'XButtons'
XButtonIDs XButtons;

// 'XButtonIDs' - Default constructor
XButtonIDs::XButtonIDs()
{
	// These values are used to index the XINPUT_Buttons array,
	// accessing the matching XINPUT button value

	A = 0;
	B = 1;
	X = 2;
	Y = 3;

	DPadUp = 4;
	DPadDown = 5;
	DPadLeft = 6;
	DPadRight = 7;

	LeftShoulder = 8;
	RightShoulder = 9;

	LeftThumbstick = 10;
	RightThumbstick = 11;

	Start = 12;
	Back = 13;
}