#pragma once

enum PlayerAction { MoveToTheLeft, MoveToTheRight, Accelerate, GetDown, Jump };
class InputDevice
{
public:
	InputDevice();
	virtual ~InputDevice();

	virtual bool IsSelected(PlayerAction playerAction) = 0;
};

