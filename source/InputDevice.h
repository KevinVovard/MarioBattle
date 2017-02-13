#pragma once

enum PlayerAction { MoveToTheLeft, MoveToTheRight, Accelerate, GetDown, Jump };
class InputDevice
{
public:
	InputDevice();
	~InputDevice();

	virtual bool IsSelected(PlayerAction playerAction) = 0;
};

