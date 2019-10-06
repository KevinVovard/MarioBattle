#include "Character.h"
#include "helpingCode.h"
#include <cmath>
#include <iostream>

// Initialization of the generator
long Character::s_objectIdGenerator = 1;

Character::Character(): 
m_heightTileReduced(21), 
speedX(0), 
speedY(0),
targetSpeedX(0), 
maxSpeed(1.5), 
maxSuperSpeed(2.5), 
factorAcceleration(0.1), 
acceleY(10), 
countJump(0), 
wasHeld(false), 
//m_baseLineTiles(0),
//m_lineTiles(1), 
//m_lastLineTiles(1),
m_isOrientedTowardsLeft(false),
m_wasOrientedTowardsLeft(false),
m_currentTile(0), 
m_isJumping(false), 
m_isDown(false), 
m_isRunning(false), 
m_isWalking(false), 
m_walkingCount(0), 
m_elapsedTimeWalkingCount(0),
m_numberCasesScan(3),
m_characterState(CharacterState_Idle)
{
	// currently the character instances are created sequentially, lock if it is not the case anymore
	m_objectId = s_objectIdGenerator++;
}

//loadResources only opens the tileset form the PNG file in a bitmap format
HRESULT Character::LoadResources(ID2D1RenderTarget *pRendertarget,IWICImagingFactory *pIWICFactory)
{
	wchar_t widearray[100];
    mbstowcs(widearray, m_fileName, 100);

	HRESULT hr = LoadBitmapFromFile(pRendertarget,pIWICFactory,widearray,&m_tileset);
	return hr;
}

//Verify if the character collides with the map
void Character::VerifyMapCollision(Map* map)
{
	//if the character is down, we change m_heightTile to the reduced height value
	int oldHeightTile = 0;
	
	//TODO: remove once adaptive height work is done
	if (m_isDown)
	{
		m_y += (m_heightTile-m_heightTileReduced);
		oldHeightTile = m_heightTile;
		m_heightTile = m_heightTileReduced;		
	}

	//start with X
	int minGridY=0;
	int maxGridY=0;
	double distX = speedX;
	minGridY = m_y/map->HAUTEUR_TILE;

	//maxGridY is equal to the top of the character tile
	//we make a conditional test for the case where the character is exactly aligned with the grid or not
	if ((int)(m_y + this->GetTileHeight()) % map->HAUTEUR_TILE == 0)
	{
		maxGridY = (m_y + this->GetTileHeight()) / map->HAUTEUR_TILE;
	}
	else
	{
		maxGridY = ((m_y+ this->GetTileHeight())/map->HAUTEUR_TILE)+1;
	}

	for (int a = minGridY ; a < maxGridY ; a++)
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

			for (int d=c;d<(c+m_numberCasesScan);d++)
			{
				int currentD = d;

				//if we are over the number of tiles of the world, we remove this number of our variable for the looping
				if (currentD >= map->nbtiles_largeur_monde)
				{
					currentD -= map->nbtiles_largeur_monde;
				}

				if(map->schema[currentD][a]==0||map->schema[currentD][a]==11)
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

					if(calcDistX2<distX)
					{
						distX=calcDistX2;
						speedX=0;
					}
				}
			}
		}//end test if speedX>0
	}

	//movement in Y!!!!!!!!!!!!!!!!!TO CHANGE
	/////////////////////////////
	int maxGridX=0;
	double distY = speedY;

	int minGridX = m_x / map->LARGEUR_TILE;

	if ((int)(m_x + m_widthTile) % map->LARGEUR_TILE == 0)
	{
		maxGridX = (m_x + m_widthTile) / map->LARGEUR_TILE;
	}
	else
	{
		maxGridX = ((m_x+m_widthTile)/map->LARGEUR_TILE)+1;
	}

	for (int a = minGridX; a < maxGridX; a++)
	{
		if (speedY < 0)
		{
			int startY = 0;
			if ((int)m_y%map->HAUTEUR_TILE == 0)
			{
				startY = m_y / map->HAUTEUR_TILE - 1;
			}
			else
			{
				startY = m_y / map->HAUTEUR_TILE;
			}

			for (int b = startY; b >= (startY - m_numberCasesScan); b--)
			{
				if (b >= 0)
				{
					if (map->schema[a % (map->nbtiles_largeur_monde)][b] == 0 || map->schema[a % (map->nbtiles_largeur_monde)][b] == 11)
					{
						double calcDistY = m_y - ((b + 1)*map->HAUTEUR_TILE);
						if (calcDistY < abs(distY))
						{
							distY = -calcDistY;
							speedY = 0.001;
							countJump = 25;
						}
					}
				}
			}
		}

		if (speedY > 0)
		{
			int c = (m_y + this->GetTileHeight()) / map->HAUTEUR_TILE;

			for (int d = c; d < map->nbtiles_hauteur_monde; d++)
			{
				if (map->schema[a % (map->nbtiles_largeur_monde)][d] == 0 || map->schema[a % (map->nbtiles_largeur_monde)][d] == 11)
				{
					if (d <= (map->nbtiles_hauteur_monde - 1))
					{
						double calcDistY = (d*map->HAUTEUR_TILE) - (m_y + this->GetTileHeight());
						if (calcDistY < distY)
						{
							distY = calcDistY;
							speedY = 0;
							m_isJumping = false;
						}
					}
				}
			}
		}
	}
		
	m_x+=distX;	
	m_y+=distY;

		//test for loop world X
	if (m_x >= (map->nbtiles_largeur_monde*map->LARGEUR_TILE))
	{
		//if we go over the world limit we remove this length of the position
		m_x -= (map->nbtiles_largeur_monde*map->LARGEUR_TILE);
	}

	if (m_x < 0)
	{
		//not totally accurate
		m_x += (map->nbtiles_largeur_monde*map->LARGEUR_TILE);
	}

	// TODO : remove once adaptive height work is done
	//we reset m_heightTile to its old value
	if (oldHeightTile != 0)
	{
		m_heightTile = oldHeightTile;
		m_y -= (m_heightTile - m_heightTileReduced);
	}
}

void Character::UpdatePosition()
{
}

int Character::nextWalkingTile(float dt, float threshold)
{
	m_elapsedTimeWalkingCount += dt;
	if (m_elapsedTimeWalkingCount > threshold)
	{
		m_walkingCount++;
		m_walkingCount %= 2;
		m_elapsedTimeWalkingCount = 0;
	}
	return (m_walkingCount + 1);
}

//Verify if the character collides with any moving objects
void Character::VerifyMovingObjectCollision(std::vector<Character*> collectionCharacter)
{
	for (int i = 0; i < collectionCharacter.size(); i++)
	{
		if(collectionCharacter.at(i)!=this)
		{
			this->CollisionObject(collectionCharacter.at(i));
		}
	}
}

void Character::CollisionObject(Character* character)
{
	//TODO: Add test for X wrapping (second character box)
	if (((this->m_x+this->GetTileWidth()) > character->m_x)
		&&this->m_x < (character->m_x+character->GetTileWidth())
		&&((this->m_y+this->GetTileHeight())>character->m_y)
		&&(this->m_y<(character->m_y+character->GetTileHeight())))
	{
		this->CollisionEffect(character);
	}
}

//specify the source image in the tileset, the source destination on the scene (with positions of the movement logic) and the splitting of the picture
void Character::Render(ID2D1RenderTarget* m_pRenderTarget, float scalingFactor, int sceneOffsetWidth, int sceneOffsetHeight, Map* map)
{
	// If the speed is superior to zero we show the characters oriented toward the right side
	if (speedX > 0)
	{
		m_isOrientedTowardsLeft = false;
		//m_lineTiles = m_baseLineTiles + 1;
	}
	// if the speed is inferior to zero we show them oriented toward the left side
	else if (speedX < 0)
	{
		m_isOrientedTowardsLeft = true;
		//m_lineTiles = m_baseLineTiles + 0;
	}
	// else if the speed is null they stay oriented in the same direction
	else
	{
		m_isOrientedTowardsLeft = m_wasOrientedTowardsLeft;
		//m_lineTiles = m_lastLineTiles;
	}

	// Source rectangle in the character tileset
	D2D1_RECT_F Rect_src1;
	Rect_src1.left = (m_currentTile) * m_widthTile;
	//Rect_src1.top = m_lineTiles * m_heightTile;
	Rect_src1.top = 0;
	Rect_src1.right = (m_currentTile + 1) * m_widthTile;
	//Rect_src1.bottom = m_heightTile * (1 + m_lineTiles);
	Rect_src1.bottom = GetTileHeight();

	// Destination rectangle on the screen
	D2D1_RECT_F Rect_dest1;
	Rect_dest1.left = (int)((m_x * scalingFactor) + sceneOffsetWidth);
	Rect_dest1.top = (int)((m_y * scalingFactor) + sceneOffsetHeight);
	Rect_dest1.right = (int)(((m_x + m_widthTile) * scalingFactor) + sceneOffsetWidth);
	Rect_dest1.bottom = (int)(((m_y + GetTileHeight())* scalingFactor) + sceneOffsetHeight);

	D2D1_RECT_F Rect_src2;
	D2D1_RECT_F Rect_dest2;
	bool isSplitted = false;
	// If the character reach the start or end of the screen there is a looping dipsay effect for showing the other half on the other side of the screen
	if (m_x + m_widthTile>(map->nbtiles_largeur_monde * map->LARGEUR_TILE))
	{
		isSplitted = true;

		Rect_dest1.right = ((map->nbtiles_largeur_monde * map->LARGEUR_TILE) * scalingFactor) + sceneOffsetWidth;
		Rect_src1.right = (m_currentTile + 1) * m_widthTile - ((m_x + m_widthTile) - map->nbtiles_largeur_monde * map->LARGEUR_TILE);

		//D2D1_RECT_F Rect_src2;
		Rect_src2.left = Rect_src1.right;
		Rect_src2.top = Rect_src1.top;
		Rect_src2.right = (m_currentTile + 1) * m_widthTile;
		Rect_src2.bottom = Rect_src1.bottom;

		//D2D1_RECT_F Rect_dest2;
		Rect_dest2.left = (int)(sceneOffsetWidth);
		Rect_dest2.top = (int)((m_y * scalingFactor) + sceneOffsetHeight);
		Rect_dest2.right = (int)((((m_x + m_widthTile) - map->nbtiles_largeur_monde * map->LARGEUR_TILE) * scalingFactor) + sceneOffsetWidth);
		Rect_dest2.bottom = (int)(((m_y + GetTileHeight()) * scalingFactor) + sceneOffsetHeight);
	}

	if (isSplitted && !m_isOrientedTowardsLeft)
	{
		const FLOAT newRectSrc2Left = Rect_src1.left;
		const FLOAT newRectSrc2Right = Rect_src1.left + (Rect_src2.right - Rect_src2.left);

		const FLOAT newRectSrc1Left = newRectSrc2Right;
		const FLOAT newRectSrc1Right = Rect_src2.right;

		Rect_src1.left = newRectSrc1Left;
		Rect_src1.right = newRectSrc1Right;

		Rect_src2.left = newRectSrc2Left;
		Rect_src2.right = newRectSrc2Right;
	}

	// Display rect 1
	if (!m_isOrientedTowardsLeft)
	{
		D2D1_POINT_2F center = D2D1::Point2F(Rect_dest1.left + (int)((Rect_dest1.right - Rect_dest1.left) / 2.0f), 0);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(-1, 1, center));
	}
	
	m_pRenderTarget->DrawBitmap(m_tileset, Rect_dest1, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, Rect_src1);
	
	if (!m_isOrientedTowardsLeft)
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); // put back regular transform

	if (isSplitted)
	{
		// Display rect 2
		if (!m_isOrientedTowardsLeft)
		{
			const D2D1_POINT_2F center = D2D1::Point2F(Rect_dest2.left + (int)((Rect_dest2.right - Rect_dest2.left) / 2.0f), 0.0f);
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(-1, 1, center));
		}

		m_pRenderTarget->DrawBitmap(m_tileset, Rect_dest2, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, Rect_src2);

		if (!m_isOrientedTowardsLeft)
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	// Records current orientation
	//m_lastLineTiles = m_lineTiles;
	m_wasOrientedTowardsLeft = m_isOrientedTowardsLeft;
}

// Set the x, y position and the orientation of the character
void Character::SetPositionAndOrientation(int x, int y, Orientation orientation)
{
	m_x = x;
	m_y = y;
	// The resource line used defines the orientation
	//m_lineTiles = orientation == Orientation::Left ? 0 : 1;
	//m_lastLineTiles = m_lineTiles;
	
	//m_wasOrientedTowardsLeft == Orientation::Left ? true : false;

	m_isOrientedTowardsLeft = orientation == Orientation::Left;
	m_wasOrientedTowardsLeft = m_isOrientedTowardsLeft;
}

// Get the tile width
int Character::GetTileWidth()
{
	return m_widthTile;
}

// Get the tile heigth
int Character::GetTileHeight()
{
	return m_heightTile;
	//return m_tileHeights[m_currentTile];
}

Character::~Character(void)
{
}
