#include "Mario.h"

Mario::Mario(InputDevice* inputDevice) : Player(inputDevice)
{
	m_fileName = "Resources/mario.png";
	m_widthTile = 16;
	m_heightTile = 28;
}


Mario::~Mario()
{
}
