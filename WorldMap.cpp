#include "stdafx.h"
#include "WorldMap.h"

WorldMap::WorldMap() : cities(), currentCity(0) { }

WorldMap::WorldMap(const vector<City>& C, int ccurrentCity) : cities(C), currentCity(ccurrentCity) { }

WorldMap::WorldMap(const WorldMap& WM) : cities(WM.cities), currentCity(WM.currentCity) { }

WorldMap& WorldMap::operator=(const WorldMap& WM)
{
	if (&WM == this) return *this;

	cities = WM.cities;
	currentCity = WM.currentCity;

	return *this;
}

WorldMap::~WorldMap() { }

City& WorldMap::getCurrentCity()
{
	return cities[currentCity];
}

void WorldMap::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path("");

	RECT rect;
	GetClientRect(hWnd, &rect);

	// Composing path based on current menu
	switch (currentScreen)
	{
	default:case ITEM_NUMBER: path = DIRECTORY + "" + FORMAT; break;
	}

	// Loading image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(hdc, &rect, hBackgroundBrush);
}

void WorldMap::resizeWindow(int cx, int cy)
{
}

void WorldMap::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
}
