#include "GameManager.h"
#include "Keyboard.h"
#include "Gamepad.h"
#include "Spawn.h"
#include "Mario.h"
#include "Luigi.h"
#include "Spiny.h"


GameManager::GameManager():
m_characterCollection(),
m_playerCollection()
{
	// We need to create and add the characters before to create the device resources using CreateDeviceResources
	
	// TODO : get playing players from selection menu where connected controllers are connected (like in Broforce)
	// Adding player 1
	this->AddPlayer(new Spawn(new Keyboard()));

	// Adding player 2
	this->AddPlayer(new Mario(new Gamepad(1)));

	// Adding player 3
	this->AddPlayer(new Luigi(new Gamepad(2)));

	// Adding player 4
	this->AddPlayer(new Spawn(new Gamepad(3)));
}


GameManager::~GameManager()
{
}

// Load the resources for the Direct X environment
HRESULT GameManager::CreateDeviceResources(ID2D1HwndRenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory)
{
	HRESULT hr = S_FALSE;

	// Load map resources
    hr = this->mapManager.LoadMap("Resources/level.txt", pRendertarget, pIWICFactory);

	// Load character resources
	for (int i = 0; i < m_characterCollection.size(); i++)
	{
		hr = hr & m_characterCollection.at(i)->LoadResources(pRendertarget, pIWICFactory);
	}
	
	// storing render resources for future resource allocation
	m_gameRenderTarget = pRendertarget;
	m_gameIWICFactory = pIWICFactory;

	return hr;
}

// Initialize the scene
void GameManager::InitializeScene(int screenWidth, int screenheight)
{
	// Resize the scene
	this->ResizeScene(screenWidth, screenheight);
}

// Update the scene logic based on time
void GameManager::UpdateScene(float dt)
{
	for (int i = 0; i < m_characterCollection.size(); i++)
	{
		// Process input (from input device or IA)
		m_characterCollection.at(i)->ProcessInput(dt);

		// Verify if the character collides with any moving object
		m_characterCollection.at(i)->VerifyMovingObjectCollision(m_characterCollection);

		// Verifies if the character collides with the map
		m_characterCollection.at(i)->VerifyMapCollision(mapManager.GetMap());
		
		// Update the current position
		m_characterCollection.at(i)->UpdatePosition();

		// Update the current tile used for display based on the time
		m_characterCollection.at(i)->UpdateTile(dt);
	}

	// TODO: Generate enemies using AddEnemy(....) function which will use m_characterCollection.push_back(new Spiny());
	m_enemyCreationTimer += dt;
	if (m_enemyCreationTimer > TIME_TO_WAIT_BEFORE_TO_CREATE_NEW_ENEMY)
	{
		this->AddEnemy();
		m_enemyCreationTimer = 0;
	}
}

// Resize the scene based on the screen size
void GameManager::ResizeScene(int screenWidth, int screenheight)
{
	Map* map = mapManager.GetMap();

	// Minimum width offset desired
	m_sceneOffsetWidth = 10;

	// minimum height offset required
	m_sceneOffsetHeight = 10;

	// Processing possible width scaling factor
	float scalingFactorX = (screenWidth - 2.0 * m_sceneOffsetWidth) / (map->nbtiles_largeur_monde*map->LARGEUR_TILE);

	// Processing possible height scaling factor
	float scalingFactorY = (screenheight - 2.0 * m_sceneOffsetHeight) / (map->nbtiles_hauteur_monde*map->HAUTEUR_TILE);

	// Keeping lowest value, the floor will round to the lowest integer
	m_sceneScalingFactor = floor(scalingFactorX <= scalingFactorY ? scalingFactorX : scalingFactorY);

	// Calculate offset in width of the scene compared to the window (for centering the scene)to apply when rendering based on the final scaling factor
	m_sceneOffsetWidth = (screenWidth - (map->nbtiles_largeur_monde*map->LARGEUR_TILE*m_sceneScalingFactor)) / 2.0;

	// Calculate offset in height of the scene compared to the window (for centering the scene)to apply when rendering based on the final scaling factor
	m_sceneOffsetHeight = (screenheight - (map->nbtiles_hauteur_monde*map->HAUTEUR_TILE*m_sceneScalingFactor)) / 2.0;
}

// Render the scene
void GameManager::RenderScene(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	/* We are currently using a scaling factor but we ould render using the window size with:
	D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);*/

	Map* map = mapManager.GetMap();

	// Render the map
	this->mapManager.RenderMap(m_pRenderTarget, m_sceneScalingFactor, m_sceneOffsetWidth, m_sceneOffsetHeight);

	// render the characters
	for (int i = 0; i < m_characterCollection.size(); i++)
	{
		m_characterCollection.at(i)->Render(m_pRenderTarget, m_sceneScalingFactor, m_sceneOffsetWidth, m_sceneOffsetHeight, map);
	}
}

void GameManager::AddPlayer(Player* player)
{
	if (m_playerCollection.size() == 0)
	{
		player->SetPositionAndOrientation(80, 132, Orientation::Right);
	}

	if (m_playerCollection.size() == 1)
	{
		player->SetPositionAndOrientation(432 - player->GetTileWidth(), 132, Orientation::Left);
	}

	if (m_playerCollection.size() == 2)
	{
		player->SetPositionAndOrientation(80, 20, Orientation::Right);
	}

	if (m_playerCollection.size() == 3)
	{
		player->SetPositionAndOrientation(432 - player->GetTileWidth(), 20, Orientation::Left);
	}
	
	// Add to the player collection
	m_playerCollection.push_back(player);

	// Add to the collection of characters to initialize, update and render
	m_characterCollection.push_back(player);
}

void GameManager::AddEnemy()
{
	Spiny* spiny = new Spiny();
	spiny->LoadResources(m_gameRenderTarget, m_gameIWICFactory);
	int min = 1;
	int max = 2;
	int randInt = min + (rand() % (int)(max - min + 1));
	if (randInt == 1)
	{
		spiny->SetPositionAndOrientation(40, 20, Orientation::Right);
	}
	else
	{
		spiny->SetPositionAndOrientation(450, 20, Orientation::Left);
	}
	
	m_characterCollection.push_back(spiny);
}

// Release the resources when the application is closed.
int GameManager::ReleaseResources()
{
	// Release the map
	mapManager.ReleaseMap();
	return 0;
}
