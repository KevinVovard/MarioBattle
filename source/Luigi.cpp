#include "Luigi.h"

Luigi::Luigi(InputDevice* inputDevice) : Player(inputDevice)
{
	m_fileName = "Resources/luigi.png";
	m_widthTile = 16;
	m_baselineTileHeight = 32;
	// TODO: get from a config file
	// TODO: to updat for 0 to 5 ? (test)
	for (int i = 0; i < 5; i++)
		m_tileHeights[i] = 32;
	
	m_tileHeights[5] = 18;//to check for this one and all below
	m_tileHeights[6] = 22;
	m_tileHeights[7] = 19;
	m_tileHeights[8] = 26;
	
	for (int i = 10; i < 20; i++)
		m_tileHeights[i] = 16;
}

Luigi::~Luigi()
{
}
