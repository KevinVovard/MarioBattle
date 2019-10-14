#include "Enemy.h"

Enemy::Enemy(void)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::SetPositionAndOrientation(int x, int y, Orientation orientation)
{
	Character::SetPositionAndOrientation(x, y, orientation);
	if (orientation == Orientation::Left)
	{
		targetSpeedX = -maxSpeed;
	}
}

void Enemy::VerifyMapCollision(Map* map)
{
	Character::VerifyMapCollision(map);

	//start with X
	int minGridY = 0;
	int maxGridY = 0;
	double distX = speedX;
	minGridY = m_y / map->HAUTEUR_TILE;

	//maxGridY is equal to the top of the character tile
	//we make a conditional test for the case where the character is exactly aligned with the grid or not
	if ((int)(m_y + this->GetTileHeight()) % map->HAUTEUR_TILE == 0)
	{
		maxGridY = (m_y + this->GetTileHeight()) / map->HAUTEUR_TILE;
	}
	else
	{
		maxGridY = ((m_y + this->GetTileHeight()) / map->HAUTEUR_TILE) + 1;
	}

	for (int a = minGridY; a < maxGridY; a++)
	{
		if (speedX < 0)
		{
			//startX is equal to the case directly at the left of the character
			int startX = 0;
			if ((int)m_x%map->LARGEUR_TILE == 0)
			{
				startX = (m_x / map->LARGEUR_TILE) - 1;
			}
			else
			{
				startX = (m_x / map->LARGEUR_TILE);
			}

			//we check from the character position to three cases before (we use the loop of the world for the X)
			for (int b = startX; b >= (startX - m_numberCasesScan); b--)
			{
				//for not changing the actual variable
				int currentB = b;
				//if b<0 we add the number of tiles of the world (width) for looping
				if (currentB < 0)
				{
					currentB += map->nbtiles_largeur_monde;
				}

				//if the case we are currently looking is an obstacle (0=>platform,11=>floor)
				if (map->schema[currentB][a] == 0 || map->schema[currentB][a] == 11)
				{
					double calcDistX = 0;
					//if the position of the character is inferior to the position of the obstacle (because of the looping)
					if (((currentB + 1)*map->LARGEUR_TILE) > m_x)
					{
						calcDistX = m_x + (map->nbtiles_largeur_monde - currentB - 1)*map->LARGEUR_TILE;//we add the width world value to m_x to compensate
					}
					else
					{
						calcDistX = m_x - ((currentB + 1)*map->LARGEUR_TILE);
					}

					if (calcDistX < abs(distX))
					{
						distX = -calcDistX;
						//NOT NECESSARY MAYBE TO ZERO SPEEDX
						speedX = 0.0;
					}
				}
			}
		}

		if (speedX>0)
		{
			//Start directly the test where the character is	
			int c = (m_x + m_widthTile) / map->LARGEUR_TILE;

			for (int d = c; d<(c + m_numberCasesScan); d++)
			{
				int currentD = d;

				//if we are over the number of tiles of the world, we remove this number of our variable for the looping
				if (currentD >= map->nbtiles_largeur_monde)
				{
					currentD -= map->nbtiles_largeur_monde;
				}

				if (map->schema[currentD][a] == 0 || map->schema[currentD][a] == 11)
				{
					double calcDistX2 = 0.0;
					if ((currentD*map->LARGEUR_TILE) < (m_x + m_widthTile))
					{
						calcDistX2 = ((currentD + map->nbtiles_largeur_monde)*map->LARGEUR_TILE) - (m_x + m_widthTile);
					}
					else
					{
						calcDistX2 = (currentD*map->LARGEUR_TILE) - (m_x + m_widthTile);
					}

					if (calcDistX2<distX)
					{
						distX = calcDistX2;
						speedX = 0;
					}
				}
			}
		}//end test if speedX>0
	}
}