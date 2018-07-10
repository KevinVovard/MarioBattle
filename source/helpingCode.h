#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <string.h>

// Structs definitions
typedef struct
{
	D2D1_RECT_F R;
	int mur;
} TileProp;

typedef struct
{
	int LARGEUR_TILE,HAUTEUR_TILE;
	int nbtilesX,nbtilesY;
	ID2D1Bitmap* tileset;
	TileProp* props;
	WORD** schema;
	int nbtiles_largeur_monde,nbtiles_hauteur_monde;
} Map;

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
// end Struct definitions

static HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRendertarget,IWICImagingFactory *pIWICFactory,PCWSTR uri, ID2D1Bitmap **ppBitmap)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	
	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri,NULL,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&pDecoder);
	
	if (SUCCEEDED(hr))
	{
	hr = pDecoder->GetFrame(0,&pSource);
	}
	
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}
	
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(pSource,GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,NULL,0.0f,WICBitmapPaletteTypeMedianCut);
	}
	
	if (SUCCEEDED(hr))
	{
		hr =pRendertarget->CreateBitmapFromWicBitmap(pConverter,NULL,ppBitmap);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;

}