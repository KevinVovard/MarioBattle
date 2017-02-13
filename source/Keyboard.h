#pragma once
#include "InputDevice.h"
#include <Windows.h>

class Keyboard : public InputDevice
{
public:
	Keyboard();

	~Keyboard();

    bool IsSelected(PlayerAction playerAction);
};

