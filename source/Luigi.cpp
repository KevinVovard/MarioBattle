#include "Luigi.h"

Luigi::Luigi(InputDevice* inputDevice) : Player(inputDevice)
{
	m_fileName = "Resources/luigi.png";
	m_widthTile = 16;
	m_heightTile = 32;
}

Luigi::~Luigi()
{
}
