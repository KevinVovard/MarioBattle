#include "Spiny.h"
#include <math.h>

Spiny::Spiny(void):
m_isFalling(false),
m_isTurning(false),
m_turningCount(0),
m_elapsedTimeTurningCount(0),
m_hasChangedOrientationDuringTurning(false)
{
	m_fileName="Resources/spiny.png";
	m_heightTile=16;
	m_widthTile=16;
	//// get value forom a config file
	//for (int i : m_tileHeights)
	//	m_tileHeights[i] = 16;
	//m_x = 32;
	//m_y = 48-m_heightTile;
	speedX = 1;
	speedY = 1;
	maxSpeed = 0.3;
	targetSpeedX = maxSpeed;
	m_isWalking = true;
	m_walkingCount = 0;
}

Spiny::~Spiny(void)
{
}

void Spiny::ProcessInput(float dt)
{
	speedX = factorAcceleration * targetSpeedX + (1.0 - factorAcceleration) * speedX;
	if (abs(speedX) < 0.01) speedX = 0.0;

	speedY += acceleY * dt;
	if (speedY > 0.2) m_isFalling = true;
	else m_isFalling = false;

	if (m_isTurning)
	{
		speedX /= 100;
		if (m_hasChangedOrientationDuringTurning == false && m_turningCount == 3)
		{
			m_hasChangedOrientationDuringTurning = true;
			targetSpeedX = -targetSpeedX;
			speedX = -speedX;
		}
	}
}

void Spiny::UpdateTile(float dt)
{
	if (m_isFalling)
	{
		m_currentTile = 2;
	}
	else if (m_isTurning)
	{
		m_currentTile = nextTurningTile(dt);
	}
	else if (m_isWalking)
	{
		m_currentTile = nextWalkingTile(dt, 0.15);
	}
}

int Spiny::nextWalkingTile(float dt, float threshold)
{
	m_elapsedTimeWalkingCount += dt;
	if (m_elapsedTimeWalkingCount > threshold)
	{
		if (m_walkingCount == 0)
		{
			m_walkingCount = 1;
		}
		else if (m_walkingCount == 1)
		{
			m_walkingCount = 0;
		}

		m_elapsedTimeWalkingCount = 0;
	}
	return (m_walkingCount);
}

int Spiny::nextTurningTile(float dt)
{
	m_elapsedTimeTurningCount += dt;

	if (m_elapsedTimeTurningCount > 0.15)
	{
		m_elapsedTimeTurningCount = 0;
		m_turningCount++;

		if (m_turningCount > 4)
		{
			m_turningCount = 0;
			m_isTurning = false;
			m_hasChangedOrientationDuringTurning = false;
			m_x += 5000 * speedX;
		}
	}

	if (!m_hasChangedOrientationDuringTurning&&m_turningCount == 3)
	{
		return 5;
	}

	if (!m_hasChangedOrientationDuringTurning)
	{
		return (m_turningCount + 3);
	}
	else
	{
		return (4 - (m_turningCount - 3));
	}
}



void Spiny::VerifyMapCollision(Map* map)
{
	Character::VerifyMapCollision(map);
	//rough test for knowing if it is the tubes below
	if (m_y > (map->nbtiles_largeur_monde*map->HAUTEUR_TILE / 2.0))
	{
		int posX = 0;
		if (speedX > 0)
		{
			posX = (m_x + m_widthTile) / map->LARGEUR_TILE;
		}
		else
		{
			posX = m_x / map->LARGEUR_TILE;
		}

		int posY = m_y / map->HAUTEUR_TILE;
		if (map->schema[posX%map->nbtiles_largeur_monde][posY] == 6)
		{
			bool a = true;
		}
	}
}


void Spiny::CollisionEffect(Character* character)
{
	Spiny* spiny = dynamic_cast<Spiny*>(character);
	if (spiny !=NULL)
	{
		m_isTurning = true;
	}
}