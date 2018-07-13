#pragma once
#include "Player.h"
#include "Enemy.h"
#include "GameManager.h"
#include "SoundManager.h"
#include <vector>

class GameManager
{
public:
	GameManager();
	~GameManager();
	
	// Load the resources for the Direct X environment
	HRESULT CreateDeviceResources(ID2D1HwndRenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory);
	
	// Initialize the scene
	void InitializeScene(int screenWidth, int screenheight);

	// Update the scene logic based on time
	void UpdateScene(float dt);

	// Resize the scene based on the screen size
	void ResizeScene(int screenWidth, int screenheight);

	// Render the scene
	void RenderScene(ID2D1HwndRenderTarget* m_pRenderTarget);
	
	// Release the resources when the application is closed.
	int ReleaseResources();

	// Adding a player to the current game
	void AddPlayer(Player* player);

	// Adding an enemy to the current game
	void AddEnemy();

	SoundEffect GetSoundEffectForCharacterState(CharacterState characterState);

private:
	// Manage the map (background of the game)
	MapManager mapManager;
	
	// Manage the music and sound effects
	SoundManager* m_soundManager;

	// Collection of characters (moving elements)
	std::vector<Character*> m_characterCollection;
	
	// collection of players
	std::vector<Player*> m_playerCollection;

	// Offset in width of the scene compared to the window(for centering the scene)to apply when rendering
	int m_sceneOffsetWidth = 0;

	// Offset in height of the scene compared to the window (for centering the scene)to apply when rendering 
	int m_sceneOffsetHeight = 0;

	// Scaling factor used for the rendering (could be set from a window menu)
	float m_sceneScalingFactor = 2;

	float m_enemyCreationTimer = 0;

	// time to wait before to create new enemy in seconds
	const int TIME_TO_WAIT_BEFORE_TO_CREATE_NEW_ENEMY = 8;

	ID2D1HwndRenderTarget *m_gameRenderTarget;
	IWICImagingFactory *m_gameIWICFactory;
};

