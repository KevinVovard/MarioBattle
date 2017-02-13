#include "Keyboard.h"


Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

bool Keyboard::IsSelected(PlayerAction playerAction)
{
	switch (playerAction)
	{
		case PlayerAction::MoveToTheLeft:
			return GetAsyncKeyState(VK_LEFT) & 0x8000;
			break;

		case PlayerAction::MoveToTheRight:
			return GetAsyncKeyState(VK_RIGHT) & 0x8000;
			break;

		case PlayerAction::Accelerate:
			return GetAsyncKeyState(0x57) & 0x8000;
			break;

		case PlayerAction::GetDown:
			return GetAsyncKeyState(VK_DOWN) & 0x8000;
			break;

		case PlayerAction::Jump:
			return GetAsyncKeyState(0x58) & 0x8000;
			break;

		default:
			return false;
			break;
	}		
}
