#pragma once
#include "helpingCode.h"
#include <stdio.h>

class MapManager
{
public:
	MapManager(void);
	~MapManager(void);

	// Load the map resources
	HRESULT LoadMap(const char* fic, ID2D1RenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory);
	
	// Render Map on the screen
	int RenderMap(ID2D1HwndRenderTarget* m_pRenderTarget, float scalingFactor, int sceneOffsetWidth, int sceneOffsetHeight);
	
	// Release map resources
	int ReleaseMap();

	
	// Retrieve the m_map object
	Map* GetMap(void);

private:
	// Load the tileset
	HRESULT LoadMap_tileset(FILE* F, ID2D1RenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory);

	// Load the resources file (in resources)
	void LoadMap_level(FILE* F, Map* map, ID2D1RenderTarget *pRendertarget);

	// Map object
	Map* m_map;
};

