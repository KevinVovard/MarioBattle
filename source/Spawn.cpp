#include "Spawn.h"
#include "Spiny.h"
#include <math.h>

Spawn::Spawn(InputDevice* inputDevice) : Player(inputDevice)
{
	m_fileName="Resources/spawn.png";
	m_widthTile=16;
	m_heightTile=28;
}

Spawn::~Spawn(void)
{
}


