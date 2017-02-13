#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include "MapManager.h"
#include <vector>

//abstract class from which players and ennemies inherite
/*

The character inheritance tree is:

                                        Characters
                                            ||
                              ==============================
                              ||                          ||
                              ||                          ||
                            Players                    Ennemies
                              ||                          ||
                          ==========             ===================
                          ||      ||             ||       ||       ||
                          ||      ||             ||       ||       ||
                        Spawn     ??            Turtle    ??       ??

The character logic loop is:

      |===============|     |======================|     |================================|     |=================|     |=============| 
      | Process Input | --> | Verify Map Collision | --> | Verify Moving Object Collision | --> | Update Position | --> | Update Tile |
      |===============|     |======================|     |================================|     |=================|     |=============| 
          ^          ^
	     /            \
        /              \
     Input devices      IA

*/

// Way toward which the character is oriented
enum Orientation {Left, Right};

class Character
{
public:
	Character(void);
	
	//loadResources only opens the tileset form the PNG file in a bitmap format
	HRESULT LoadResources(ID2D1RenderTarget *pRendertarget,IWICImagingFactory *pIWICFactory);
		
	//specify the source image in the tileset, the source destination on the scene (with positions of the movement logic) and the splitting of the picture
	void Render(ID2D1RenderTarget* m_pRenderTarget, float scalingFactor, int sceneOffsetWidth, int sceneOffsetHeight, Map* map);
	
	//Verify if the character collides with the map
	virtual void VerifyMapCollision(Map* map);	

	//Verify if the character collides with any moving objects
	void VerifyMovingObjectCollision(std::vector<Character*> collectionCharacter);
	
	void CollisionObject(Character* character);

	virtual void UpdatePosition();
	
	virtual void CollisionEffect(Character* character) = 0;
	
	//select which tile to draw base on the current state, also contains logic for walking and running animation
	virtual void UpdateTile(float dt) = 0;
	
	//update the movement logic of the character based on the time delta
	virtual void ProcessInput(float dt) = 0;

	// Set the x, y position and the orientation of the character
	virtual void SetPositionAndOrientation(int x, int y, Orientation orientation);

	// Get the tile width
	int GetTileWidth();

	// Get the tile height
	int GetTileHeight();

	~Character(void);

	protected:
	int nextWalkingTile(float dt,float threshold);

	ID2D1Bitmap* m_tileset;
	double factorAcceleration;
	double m_x;
	double m_y;
	double speedX;
	double speedY;
	double acceleY;
	double targetSpeedX;
	double maxSpeed;
	double maxSuperSpeed;
	const char* m_fileName;
	bool wasHeld;
	int m_lineTiles;
	int m_lastLineTiles;
	int countJump;
	int m_widthTile;
	int m_heightTile;
	int m_heightTileReduced;
	int m_currentTile;
	int m_walkingCount;
	int m_numberCasesScan;
	double m_elapsedTimeWalkingCount;
	bool m_isJumping;
	bool m_isDown;
	bool m_isRunning;
	bool m_isWalking;
};

