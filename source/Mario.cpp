#include "Mario.h"

Mario::Mario(InputDevice* inputDevice) : Player(inputDevice)
{
	m_fileName = "Resources/mario.png";
	m_widthTile = 16;
	m_baselineTileHeight = 28;
	// TODO: get from a config file
	// TODO: to updat for 0 to 5 ? (test)
	for (int i = 0; i < 5; i++)
		m_tileHeights[i] = 28;
	
	m_tileHeights[5] = 18;
	m_tileHeights[6] = 22;
	m_tileHeights[7] = 19;
	m_tileHeights[8] = 26;
	
	for (int i = 9; i < 16; i++)
		m_tileHeights[i] = 16;
}


Mario::~Mario()
{
}
