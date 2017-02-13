#include <string.h>
#define _CRT_SECURE_NO_DEPRECATE 
#include "MapManager.h"

#define CACHE_SIZE 5000

MapManager::MapManager(void) :m_map(NULL)
{
}

// Load the map resources
HRESULT MapManager::LoadMap(const char* level, ID2D1RenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory)
{
	FILE* file = fopen(level, "r");
	if (!file)
	{
		printf("file %s not found !! \n", level);
		system("pause");
		exit(-1);
	}

	m_map = new Map;
	HRESULT hr = LoadMap_tileset(file, pRendertarget, pIWICFactory);
	if SUCCEEDED(hr)
	{
		LoadMap_level(file, m_map, pRendertarget);
	}
	fclose(file);

	return hr;
}

// Retrieve the m_map object
Map* MapManager::GetMap(void)
{
	return m_map;
}

// Render Map on the screen
int MapManager::RenderMap(ID2D1HwndRenderTarget* m_pRenderTarget, float scalingFactor, int sceneOffsetWidth, int sceneOffsetHeight)
{
	int i, j;
	D2D1_RECT_F Rect_dest;
	int tileNumber;
	for (i = 0; i < m_map->nbtiles_largeur_monde; i++)
	{
		for (j = 0; j < m_map->nbtiles_hauteur_monde; j++)
		{
			Rect_dest.left = (i * m_map->LARGEUR_TILE * scalingFactor) + sceneOffsetWidth;
			Rect_dest.top = (j * m_map->HAUTEUR_TILE * scalingFactor) + sceneOffsetHeight;
			Rect_dest.right = ((i + 1) * m_map->LARGEUR_TILE * scalingFactor) + sceneOffsetWidth;
			Rect_dest.bottom = ((j + 1) * m_map->HAUTEUR_TILE * scalingFactor) + sceneOffsetHeight;
			tileNumber = m_map->schema[i][j];
			m_pRenderTarget->DrawBitmap(m_map->tileset, Rect_dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &(m_map->props[tileNumber].R));
		}
	}

	return 0;
}

// Release map resources
int MapManager::ReleaseMap()
{
	int i;
	SafeRelease(&(m_map->tileset));
	for (i = 0; i < m_map->nbtiles_hauteur_monde; i++)
	{
		free(m_map->schema[i]);
	}

	free(m_map->schema);
	free(m_map->props);
	free(m_map);
	return 0;
}

MapManager::~MapManager(void)
{
}

/* private functions */

// Load the tileset
HRESULT MapManager::LoadMap_tileset(FILE* file, ID2D1RenderTarget *pRendertarget, IWICImagingFactory *pIWICFactory)
{
	int tileNumber, i, j;
	char buf[CACHE_SIZE]; 
	char buf2[CACHE_SIZE]; 
	fscanf(file, "%s", buf); // #tileset
	fscanf(file, "%s", buf); // read the file location
	wchar_t widearray[100];
	mbstowcs(widearray, buf, 100);
	HRESULT hr = LoadBitmapFromFile(pRendertarget, pIWICFactory, widearray, &(m_map->tileset));
	//Read the number of tiles in X and the number of tiles in Y
	fscanf(file, "%d %d", &m_map->nbtilesX, &m_map->nbtilesY);
	m_map->LARGEUR_TILE = m_map->tileset->GetSize().width / m_map->nbtilesX;
	m_map->HAUTEUR_TILE = m_map->tileset->GetSize().height / m_map->nbtilesY;
	m_map->props = new TileProp[m_map->nbtilesX * m_map->nbtilesY];

	for (j = 0, tileNumber = 0; j<m_map->nbtilesY; j++)
	{
		for (i = 0; i<m_map->nbtilesX; i++, tileNumber++)
		{
			m_map->props[tileNumber].R.left = i * m_map->LARGEUR_TILE;
			m_map->props[tileNumber].R.top = j * m_map->HAUTEUR_TILE;
			m_map->props[tileNumber].R.right = (i + 1) * m_map->LARGEUR_TILE;
			m_map->props[tileNumber].R.bottom = (j + 1) * m_map->HAUTEUR_TILE;
			fscanf(file, "%s %s", buf, buf2);
			m_map->props[tileNumber].mur = 0;
			if (strcmp(buf2, "mur") == 0)
			{
				m_map->props[tileNumber].mur = 1;
			}
		}
	}

	return hr;
}

// Load the resources file (in resources)
void MapManager::LoadMap_level(FILE* F, Map* m_map, ID2D1RenderTarget *pRendertarget)
{
	int i, j;
	char buf[CACHE_SIZE];  // un buffer, petite mémoire cache
	fscanf(F, "%s", buf); // #level
	fscanf(F, "%d %d", &m_map->nbtiles_largeur_monde, &m_map->nbtiles_hauteur_monde);

	m_map->schema = new unsigned short*[m_map->nbtiles_largeur_monde];
	for (i = 0; i < m_map->nbtiles_largeur_monde; i++)
	{
		m_map->schema[i] = new unsigned short[m_map->nbtiles_hauteur_monde];
	}

	for (j = 0; j < m_map->nbtiles_hauteur_monde; j++)
	{
		for (i = 0; i < m_map->nbtiles_largeur_monde; i++)
		{
			int tmp;
			fscanf(F, "%d", &tmp);
			if (tmp >= m_map->nbtilesX * m_map->nbtilesY)
			{
				printf("level tile hors limite\n");
				system("pause");
				exit(-1);
			}
			m_map->schema[i][j] = tmp;
		}
	}
}

