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

void WorldMap::drawWindow(HDC hdc, int cx, int cy)
{
}

void WorldMap::resizeWindow(int cx, int cy)
{
}

void WorldMap::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
}
